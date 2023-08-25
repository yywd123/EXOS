#include <exos/panic.hpp>
#include <exos/logger.hpp>
#include <platform/platform.hpp>

USE(EXOS::Utils);

void
__panic(const char *msg, const char *sourceFileName, const char *functionName, uint32_t line, InterruptFrame *frame) {
	Logger::log(Logger::FATAL, "!!! kernel panic at @:@(@) !!!\ncause: @", sourceFileName, functionName, (int32_t)line, msg);
	if(frame) {
		Logger::print("Register Info:\n");
		Logger::printf("rax @, rbx @, rcx @, rdx @\n", frame->regs.rax, frame->regs.rbx, frame->regs.rcx, frame->regs.rdx);
		Logger::printf("rsi @, rdi @, rsp @, rbp @\n", frame->regs.rsi, frame->regs.rdi, frame->rsp, frame->regs.rbp);
		Logger::printf("r8 @, r9 @, r10 @, r11 @\n", frame->regs.r8, frame->regs.r9, frame->regs.r10, frame->regs.r11);
		Logger::printf("r12 @, r13 @, r14 @, r15 @\n", frame->regs.r12, frame->regs.r13, frame->regs.r14, frame->regs.r15);
	} else {
		Logger::log(Logger::INFO, "this panic without any register info");
	}

	Logger::printf("core ID: @\n", Platform::Processor::getCurrentCoreApicID());
	uintptr_t kernelBase = 0;
	uint64_t kernelLimit = 0;
	getAddressFromSymbol(kernelBase, "_imageBase");
	getAddressFromSymbol(kernelLimit, "_imageEnd");
	Logger::printf("kernel base 0x@", kernelBase);
	if(frame) {
		if(frame->rip < kernelBase || frame->rip > kernelLimit) {
			Logger::printf(", panic at rip @\n", frame->rip);
		} else {
			Logger::printf(", panic at kernel offset 0x@\n", frame->rip - kernelBase);
		}
	} else
		Logger::print('\n');

	Logger ::log(Logger::INFO, "error dumped. halting.");
	while(true) ASM("hlt");
}