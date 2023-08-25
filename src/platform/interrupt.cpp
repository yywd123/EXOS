#include <platform/platform.hpp>
#include <exos/logger.hpp>
#include <exos/panic.hpp>
#include <platform/platform.hpp>
#include <exos/keyboard.hpp>

USE(EXOS::Utils);

static Fn<void, const InterruptFrame *> interruptHandlers[256] = {0};

__NAMESPACE_DECL(Platform::Interrupt)

void
initialize(InterruptDescriptor *idt) {
	uintptr_t fn = 0;
#define SET_ENTRY(index, isException)                \
	getAddressFromSymbol(fn, "interruptEntry" #index); \
	idt[0x##index].set(fn, 8, 0, (isException) ? 0x8f : 0x8e)

	SET_ENTRY(00, true);
	SET_ENTRY(01, true);
	SET_ENTRY(03, true);
	SET_ENTRY(04, true);
	SET_ENTRY(05, true);
	SET_ENTRY(06, true);
	SET_ENTRY(07, true);
	SET_ENTRY(08, true);
	SET_ENTRY(0a, true);
	SET_ENTRY(0b, true);
	SET_ENTRY(0c, true);
	SET_ENTRY(0d, true);
	SET_ENTRY(0e, true);
	SET_ENTRY(10, true);
	SET_ENTRY(11, true);
	SET_ENTRY(12, true);
	SET_ENTRY(13, true);
	SET_ENTRY(14, false);
	SET_ENTRY(15, false);
	SET_ENTRY(16, false);
	SET_ENTRY(17, false);
	SET_ENTRY(18, false);
	SET_ENTRY(19, false);
	SET_ENTRY(1a, false);
	SET_ENTRY(1b, false);
	SET_ENTRY(1c, false);
	SET_ENTRY(1d, false);
	SET_ENTRY(1e, false);
	SET_ENTRY(1f, false);
	SET_ENTRY(20, false);
	SET_ENTRY(21, false);
	SET_ENTRY(22, false);
	SET_ENTRY(23, false);
	SET_ENTRY(24, false);
	SET_ENTRY(25, false);
	SET_ENTRY(26, false);
	SET_ENTRY(27, false);
	SET_ENTRY(28, false);
	SET_ENTRY(29, false);
	SET_ENTRY(2a, false);
	SET_ENTRY(2b, false);
	SET_ENTRY(2c, false);
	SET_ENTRY(2d, false);
	SET_ENTRY(2e, false);
	SET_ENTRY(2f, false);
	SET_ENTRY(30, false);
	SET_ENTRY(31, false);
	SET_ENTRY(32, false);
	SET_ENTRY(33, false);
	SET_ENTRY(34, false);
	SET_ENTRY(35, false);
	SET_ENTRY(36, false);
	SET_ENTRY(37, false);
	SET_ENTRY(38, false);
	SET_ENTRY(39, false);
	SET_ENTRY(3a, false);
	SET_ENTRY(3b, false);
	SET_ENTRY(3c, false);
	SET_ENTRY(3d, false);
	SET_ENTRY(3e, false);
	SET_ENTRY(3f, false);
	SET_ENTRY(40, false);
	SET_ENTRY(41, false);
	SET_ENTRY(42, false);
	SET_ENTRY(43, false);
	SET_ENTRY(44, false);
	SET_ENTRY(45, false);
	SET_ENTRY(46, false);
	SET_ENTRY(47, false);
	SET_ENTRY(48, false);
	SET_ENTRY(49, false);
	SET_ENTRY(4a, false);
	SET_ENTRY(4b, false);
	SET_ENTRY(4c, false);
	SET_ENTRY(4d, false);
	SET_ENTRY(4e, false);
	SET_ENTRY(4f, false);
	SET_ENTRY(50, false);
	SET_ENTRY(51, false);
	SET_ENTRY(52, false);
	SET_ENTRY(53, false);
	SET_ENTRY(54, false);
	SET_ENTRY(55, false);
	SET_ENTRY(56, false);
	SET_ENTRY(57, false);
	SET_ENTRY(58, false);
	SET_ENTRY(59, false);
	SET_ENTRY(5a, false);
	SET_ENTRY(5b, false);
	SET_ENTRY(5c, false);
	SET_ENTRY(5d, false);
	SET_ENTRY(5e, false);
	SET_ENTRY(5f, false);
	SET_ENTRY(60, false);
	SET_ENTRY(61, false);
	SET_ENTRY(62, false);
	SET_ENTRY(63, false);
	SET_ENTRY(64, false);
	SET_ENTRY(65, false);
	SET_ENTRY(66, false);
	SET_ENTRY(67, false);
	SET_ENTRY(68, false);
	SET_ENTRY(69, false);
	SET_ENTRY(6a, false);
	SET_ENTRY(6b, false);
	SET_ENTRY(6c, false);
	SET_ENTRY(6d, false);
	SET_ENTRY(6e, false);
	SET_ENTRY(6f, false);
	SET_ENTRY(70, false);
	SET_ENTRY(71, false);
	SET_ENTRY(72, false);
	SET_ENTRY(73, false);
	SET_ENTRY(74, false);
	SET_ENTRY(75, false);
	SET_ENTRY(76, false);
	SET_ENTRY(77, false);
	SET_ENTRY(78, false);
	SET_ENTRY(79, false);
	SET_ENTRY(7a, false);
	SET_ENTRY(7b, false);
	SET_ENTRY(7c, false);
	SET_ENTRY(7d, false);
	SET_ENTRY(7e, false);
	SET_ENTRY(7f, false);
	SET_ENTRY(80, false);
	SET_ENTRY(81, false);
	SET_ENTRY(82, false);
	SET_ENTRY(83, false);
	SET_ENTRY(84, false);
	SET_ENTRY(85, false);
	SET_ENTRY(86, false);
	SET_ENTRY(87, false);
	SET_ENTRY(88, false);
	SET_ENTRY(89, false);
	SET_ENTRY(8a, false);
	SET_ENTRY(8b, false);
	SET_ENTRY(8c, false);
	SET_ENTRY(8d, false);
	SET_ENTRY(8e, false);
	SET_ENTRY(8f, false);
	SET_ENTRY(90, false);
	SET_ENTRY(91, false);
	SET_ENTRY(92, false);
	SET_ENTRY(93, false);
	SET_ENTRY(94, false);
	SET_ENTRY(95, false);
	SET_ENTRY(96, false);
	SET_ENTRY(97, false);
	SET_ENTRY(98, false);
	SET_ENTRY(99, false);
	SET_ENTRY(9a, false);
	SET_ENTRY(9b, false);
	SET_ENTRY(9c, false);
	SET_ENTRY(9d, false);
	SET_ENTRY(9e, false);
	SET_ENTRY(9f, false);
	SET_ENTRY(a0, false);
	SET_ENTRY(a1, false);
	SET_ENTRY(a2, false);
	SET_ENTRY(a3, false);
	SET_ENTRY(a4, false);
	SET_ENTRY(a5, false);
	SET_ENTRY(a6, false);
	SET_ENTRY(a7, false);
	SET_ENTRY(a8, false);
	SET_ENTRY(a9, false);
	SET_ENTRY(aa, false);
	SET_ENTRY(ab, false);
	SET_ENTRY(ac, false);
	SET_ENTRY(ad, false);
	SET_ENTRY(ae, false);
	SET_ENTRY(af, false);
	SET_ENTRY(b0, false);
	SET_ENTRY(b1, false);
	SET_ENTRY(b2, false);
	SET_ENTRY(b3, false);
	SET_ENTRY(b4, false);
	SET_ENTRY(b5, false);
	SET_ENTRY(b6, false);
	SET_ENTRY(b7, false);
	SET_ENTRY(b8, false);
	SET_ENTRY(b9, false);
	SET_ENTRY(ba, false);
	SET_ENTRY(bb, false);
	SET_ENTRY(bc, false);
	SET_ENTRY(bd, false);
	SET_ENTRY(be, false);
	SET_ENTRY(bf, false);
	SET_ENTRY(c0, false);
	SET_ENTRY(c1, false);
	SET_ENTRY(c2, false);
	SET_ENTRY(c3, false);
	SET_ENTRY(c4, false);
	SET_ENTRY(c5, false);
	SET_ENTRY(c6, false);
	SET_ENTRY(c7, false);
	SET_ENTRY(c8, false);
	SET_ENTRY(c9, false);
	SET_ENTRY(ca, false);
	SET_ENTRY(cb, false);
	SET_ENTRY(cc, false);
	SET_ENTRY(cd, false);
	SET_ENTRY(ce, false);
	SET_ENTRY(cf, false);
	SET_ENTRY(d0, false);
	SET_ENTRY(d1, false);
	SET_ENTRY(d2, false);
	SET_ENTRY(d3, false);
	SET_ENTRY(d4, false);
	SET_ENTRY(d5, false);
	SET_ENTRY(d6, false);
	SET_ENTRY(d7, false);
	SET_ENTRY(d8, false);
	SET_ENTRY(d9, false);
	SET_ENTRY(da, false);
	SET_ENTRY(db, false);
	SET_ENTRY(dc, false);
	SET_ENTRY(dd, false);
	SET_ENTRY(de, false);
	SET_ENTRY(df, false);
	SET_ENTRY(e0, false);
	SET_ENTRY(e1, false);
	SET_ENTRY(e2, false);
	SET_ENTRY(e3, false);
	SET_ENTRY(e4, false);
	SET_ENTRY(e5, false);
	SET_ENTRY(e6, false);
	SET_ENTRY(e7, false);
	SET_ENTRY(e8, false);
	SET_ENTRY(e9, false);
	SET_ENTRY(ea, false);
	SET_ENTRY(eb, false);
	SET_ENTRY(ec, false);
	SET_ENTRY(ed, false);
	SET_ENTRY(ee, false);
	SET_ENTRY(ef, false);
	SET_ENTRY(f0, false);
	SET_ENTRY(f1, false);
	SET_ENTRY(f2, false);
	SET_ENTRY(f3, false);
	SET_ENTRY(f4, false);
	SET_ENTRY(f5, false);
	SET_ENTRY(f6, false);
	SET_ENTRY(f7, false);
	SET_ENTRY(f8, false);
	SET_ENTRY(f9, false);
	SET_ENTRY(fa, false);
	SET_ENTRY(fb, false);
	SET_ENTRY(fc, false);
	SET_ENTRY(fd, false);
	SET_ENTRY(fe, false);
	SET_ENTRY(ff, false);

#undef SET_ENTRY
}

void
setHandler(uint8_t index, Fn<void, const InterruptFrame *> handler) {
	interruptHandlers[index] = handler;
}

extern "C" InterruptFrame *
handleInterrupt(InterruptFrame *frame) {
	// Logger::log(Logger::DEBUG, "irq @", frame->irqIndex);
	if(interruptHandlers[frame->irqIndex]) {
		interruptHandlers[frame->irqIndex](frame);
	} else {
		if(frame->irqIndex < 0x20) {
			Logger::log(Logger::FATAL, "hardware exception @", frame->irqIndex);
			panicWithReg("unhandled hardware exception", frame);
		}
	}
	return frame;
}

__NAMESPACE_END