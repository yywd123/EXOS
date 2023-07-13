#include <exos/panic.hpp>
#include <exos/logger.hpp>

USE(EXOS::Utils);

void
__panic(const char *msg, const char *sourceFileName, const char *functionName, uint32_t line, RegisterFrame *regFrame) {
	Logger::log(Logger::FATAL, "!!! kernel panic at @:@(@) !!!\ncause: @", sourceFileName, functionName, (int32_t)line, msg);
	if(regFrame) {
	} else {
		Logger::log(Logger::INFO, "this panic without any register info");
	}

	Logger::log(Logger::INFO, "error dumped. halting.");
	while(true) ASM("hlt");
}