#include <exos/shell.hpp>
#include <exos/efifb.hpp>
#include <exos/fbcon.hpp>
#include <exos/logger.hpp>
#include <utils/string.hpp>
#include <efi/efi.hpp>

USE(EXOS::Utils);
USE(EXOS::Drivers);

__NAMESPACE_DECL(Shell)

static std::vector<Command *> *commandList = nullptr;

static void
time(const char *cmdLine) {
	if(String::startsWith(cmdLine + 5, "show ")) {
		Logger::printf("@", CMOS::getTime());
	} else if(String::startsWith(cmdLine + 5, "setoff ")) {
		if(length(cmdLine + 12) < 4) {
			Logger::printf("parameters aren't enough\n");
			return;
		}
		int8_t hourOffset = (int8_t)String::parseInt(cmdLine + 12);
		int paramOffset = 1;
		if(hourOffset < 0) ++paramOffset;
		if(hourOffset >= 10) ++paramOffset;
		uint8_t minuteOffset = (uint8_t)String::parseInt(cmdLine + paramOffset);
		CMOS::setTimeOffset(hourOffset, minuteOffset);
	} else {
		Logger::print("usage: time [option] ...\n\n");
		Logger::print("options:\n");
		Logger::print("\tshow\t\t\t\tshow current time\n");
		Logger::print("\tsetoff [h] [m]\t\tsets time offset\n");
	}
}

static void
kinfo(const char *) {
	FbConsole::setColor(false, 0x39c5bb);
	Logger::printf("EXOS (build in @ @)", __DATE__, __TIME__);
	FbConsole::setColor(false, 0xb8b8b8);
	Logger::print('\n');
	Logger::print("Developing by yywd_123(");
	FbConsole::setColor(false, 0x228b22);
	Logger::print("https://space.bilibili.com/689917252");
	FbConsole::setColor(false, 0xb8b8b8);
	Logger::print(")\nGit repository: ");
	FbConsole::setColor(false, 0x228b22);
	Logger::print("https://gitee.com/yywd123/EXOS\n");
	FbConsole::setColor(false, 0xb8b8b8);
}

static void
help(const char *) {
	Logger::print("help:\n");
	_iter(commandList->size()) {
		Command *command = (*commandList)[i];
		Logger::printf("\n@\b: @", command->name, String::compare(command->description, "") ? "none" : command->description);
	}
}

static void
powerctl(const char *cmdLine) {
	if(String::startsWith(cmdLine + 9, "reboot ")) {
		eficall(gRT->ResetSystem, EFI::ResetCold, 0, 0, nullptr);
	} else if(String::startsWith(cmdLine + 9, "poweroff ")) {
		eficall(gRT->ResetSystem, EFI::ResetShutdown, 0, 0, nullptr);
	} else {
		Logger::print("usage: powerctl [option]\n\n");
		Logger::print("options:\n");
		Logger::print("\treboot\t\t\t\treboot the computer\n");
		Logger::print("\tpoweroff\t\t\tshutdown the computer\n");
	}
}

static void
clear(const char *) {
	EfiFb::drawRect({0, 0}, EfiFb::getSize(), 0);
	FbConsole::setCursorPos({0, 0});
}

static void
echo(const char *cmdLine) {
	Logger::printf("@\n", cmdLine + 5);
}

void
registerCommand(Command *cmd) {
	commandList->push_back(cmd);
}

static void
printShellHeader() {
	FbConsole::print("\n> ");
	FbConsole::setLineBase();
}

void
initilaize() {
	commandList = new std::vector<Command *>();

	registerCommand(new Command{
			"time ",
			"time settings",
			time});

	registerCommand(new Command{
			"kinfo ",
			"show kernel info",
			kinfo});

	registerCommand(new Command{
			"clear ",
			"clear the console",
			clear});

	registerCommand(new Command{
			"echo ",
			"display a string",
			echo});

	registerCommand(new Command{
			"powerctl ",
			"changing hardware power state",
			powerctl});

	registerCommand(new Command{
			"help ",
			"display this help message",
			help});
}

static const char *
trimLeadingSpace(const char *str) {
	const char *s = str;
	while(*s == ' ') ++s;
	return s;
}

void
exec(const char *cmd) {
	cmd = trimLeadingSpace(cmd);
	_iter(commandList->size()) {
		Command *command = (*commandList)[i];
		if(String::startsWith(cmd, command->name)) {
			command->execute(cmd);
			goto end;
		}
	}

	if(length(cmd)) Logger::printf("\ncommand not found: [@]", cmd);
end:
	printShellHeader();
}

__NAMESPACE_END
