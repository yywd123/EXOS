#include <platform/platform.hpp>
#include <exos/keyboard.hpp>
#include <exos/fbcon.hpp>
#include <exos/shell.hpp>
#include <deque>

USE(EXOS::Platform);
USE(std);

static deque<char> *inputQueue = nullptr;

__NAMESPACE_DECL(Drivers::Keyboard)

typedef union {
	uint8_t rawData;
	struct {
		uint8_t scanCode : 7;
		bool isReleased : 1;
	};
} PS2ScanCode;

static char keytable[0x54] = {
		0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
		'\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
		10, 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',
		0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*',
		0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, '7', 'D', '8', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};
static char keytable1[0x54] = {
		0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
		'\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
		10, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
		0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
		0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};

static uint8_t overrideCount = 0;

static uint16_t
ps2GetDisplayableKeyCode(uint8_t keyData) {
	if(keyData == 0x2a && overrideCount) {
		overrideCount = 2;
		return 0xffff;
	}
	if(overrideCount) {
		--overrideCount;
		return 0xffff;
	}
	PS2ScanCode code = {.rawData = keyData};
	if(keyData == 0xe0) {
		overrideCount = 1;
		return 0xffff;
	}
	if(keyData == 0xe1) {
		overrideCount = 5;
		return 0xffff;
	}
	return (uint16_t)code.scanCode | (code.isReleased ? BIT(15) : 0);
}

static bool capsLock = false;
static bool shift = false;

void
nextLine() {
	inputQueue->push_back(0);

	Shell::exec(&inputQueue->front());

	size_t size = inputQueue->size();
	_iter(size)
		inputQueue->pop_back();
}

void
updateKeyboardState(uint8_t keyData) {
	uint16_t c = ps2GetDisplayableKeyCode(keyData);
	if(c == 0xffff) return;

	if((c & 0xff) == KEY_CAPSLOCK && (c & BIT(15))) {
		capsLock = !capsLock;
		return;
	} else if((c & 0xff) == KEY_LSHIFT || (c & 0xff) == KEY_RSHIFT) {
		shift = !shift;
		return;
	}
	if(!(c & BIT(15) && (c & 0xff) < 0x54)) {
		char tmp = (capsLock != shift ? keytable : keytable1)[(c & 0xff)];
		if(tmp == '\b') {
			if(inputQueue->size()) inputQueue->pop_back();
			FbConsole::print("\b \b");	//  好粗暴的实现
		} else {
			inputQueue->push_back(tmp == '\n' ? ' ' : tmp);
			FbConsole::print(tmp);
		}

		if(tmp == '\n') {
			nextLine();
		}
	}
}

void
waitForKeyboardController() {
	while(IO::inb(0x64) & BIT(2))
		;
}

static void
keyboardHandler(const InterruptFrame *) {
	updateKeyboardState(IO::inb(0x60));
	Apic::ioApicEdgeAck();
}

void
initialize() {
	inputQueue = new deque<char>();

	Interrupt::setHandler(0x21, keyboardHandler);

	Apic::IOApicRTE(0x21).apply(1);

	waitForKeyboardController();
	IO::outb(0x64, 0x60);
	waitForKeyboardController();
	IO::outb(0x60, 0x47);
}

__NAMESPACE_END