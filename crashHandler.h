#ifndef CRASH_HANDLER_H
#define CRASH_HANDLER_H

#include <windows.h>

#define _O_U8TEXT 0x400000
#define CHECK_DELAY 4000

extern char PROCESS_NAME[256];

void startProcess();

#endif