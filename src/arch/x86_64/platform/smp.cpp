#include <smp.hpp>

USE(EXOS::Platform);

static SMP::SMPCore *coreList = nullptr;
static uint8_t apCoreCount = 0;

void __INIT
initBootstrapProcessor() {

}
