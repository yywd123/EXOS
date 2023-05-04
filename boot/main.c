#include <utils.h>

typedef struct
{
  unsigned char	e_ident[16];	/* Magic number and other info */
  uint16_t	e_type;			/* Object file type */
  uint16_t	e_machine;		/* Architecture */
  uint32_t	e_version;		/* Object file version */
  uint64_t	e_entry;		/* Entry point virtual address */
  uint64_t	e_phoff;		/* Program header table file offset */
  uint64_t	e_shoff;		/* Section header table file offset */
  uint32_t	e_flags;		/* Processor-specific flags */
  uint16_t	e_ehsize;		/* ELF header size in bytes */
  uint16_t	e_phentsize;		/* Program header table entry size */
  uint16_t	e_phnum;		/* Program header table entry count */
  uint16_t	e_shentsize;		/* Section header table entry size */
  uint16_t	e_shnum;		/* Section header table entry count */
  uint16_t	e_shstrndx;		/* Section header string table index */
} Elf64_Ehdr;

EFIAPI
EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
  InitializeLib(imageHandle, systemTable);
  
  EFI_FILE_PROTOCOL *rootFs = openRootFs(imageHandle);
  EFI_PHYSICAL_ADDRESS kernelAddress = 0x400000;
  UINTN pages = readFile(rootFs, L"\\EXOS\\kernel.sys", &kernelAddress, EfiLoaderCode, 0, 0);
  EFI_PHYSICAL_ADDRESS logoAddress = 0x200000;
  readFile(rootFs, L"\\efi\\EXOS\\bootres", &logoAddress, EfiLoaderData, 0, 0);
  EFI_PHYSICAL_ADDRESS vidAddress = 0x600000;
  readFile(rootFs, L"\\video", &vidAddress, EfiLoaderData, 0, 0);
  call(rootFs->Close, rootFs);

  kernelParam.efiRuntimeService = systemTable->RuntimeServices;

  switchToBestResolution(imageHandle);
  
  kernelParam.efiConfigurationTable = systemTable->ConfigurationTable;
  kernelParam.tableEntryCount = systemTable->NumberOfTableEntries;

  uint64_t entryCount;
  uint64_t mapKey;
  uint64_t descSize;
  uint32_t descVersion;
  //EFI_PHYSICAL_ADDRESS pdpt = createPageTable(getMemoryMap(&entryCount, &mapKey, &descSize, &descVersion));
  getMemoryMap(&entryCount, &mapKey, &descSize, &descVersion);
  // asm("movq %%cr3, %%rax\n\t"
  //     "andq $0b11000, %%rax\n\t"
  //     "orq %0, %%rax\n\t"
  //     "movq %%rax, %%cr3"::"b"(pdpt));
  
  // mapMemory(kernelAddress, 0, 1);
  call(gBS->ExitBootServices,imageHandle, mapKey);
  // systemTable->RuntimeServices->SetVirtualAddressMap(entryCount * descSize, descSize, descVersion, kernelParam.efiMemoryMap);

  ((void (*)(BootConfig*))((Elf64_Ehdr*)kernelAddress)->e_entry)(&kernelParam);

  while (1);

  return EFI_SUCCESS;
}
