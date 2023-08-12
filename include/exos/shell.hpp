#pragma once

__NAMESPACE_DECL(Shell)

typedef struct {
	const char *name;	 //  必须以空格结尾
	const char *description;
	Fn<void, const char *> execute;
} Command;

void
initilaize();

void
registerCommand(Command *cmd);

void
exec(const char *cmd);

__NAMESPACE_END