#include <efi/efi.h>

void *operator new(size_t size) {
  return efiAllocatePool(size);
}

void *operator new[](size_t size) {
  return efiAllocatePool(size);
}

void operator delete(void *p) {
  efiFreePool(p);
}

void operator delete(void *p, size_t) {
  efiFreePool(p);
}
