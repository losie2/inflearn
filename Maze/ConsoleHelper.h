#pragma once

#include <Windows.h>
#include "Types.h"

enum class ConsoleColor
{
	BLACK = 0,
	RED = FOREGROUND_RED,
	GREEN = FOREGROUND_GREEN,
	BLUE = FOREGROUND_BLUE,
	YELLOW = RED | GREEN,
	WHITE = RED | GREEN | BLUE,
};

class ConsoleHelper
{
public:
	static void SecCursorPosition(int32 x, int32 y);
	static void setCursorColor(ConsoleColor);
	static void ShowConsoleCursor(bool flag);
};

