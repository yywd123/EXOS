#include <mm/paging.hpp>
#include <platform/platform.hpp>
#include <exos/logger.hpp>

USE(EXOS::Utils);

__NAMESPACE_DECL(Memory::Paging)

static bool isPML5support = false;

//  支持5级分页就是pml5 不然是pml4
static uint64_t __aligned(PAGE_SIZE) pageMapLevelMax[1024] = {0};

void __INIT
initialize(uint64_t pageCount) {
	uint32_t tmp;
	uint32_t ecx;
	__cpuid(0x7, tmp, tmp, ecx, tmp);

	isPML5support = (ecx & BIT(16) != 0);
	Logger::log(Logger::DEBUG, "pml5 support = @", isPML5support);
}

bool
mapPages(uint64_t pindex, uint64_t vindex, uint64_t pageCount, uint64_t flags) {
	return true;
}

__NAMESPACE_END