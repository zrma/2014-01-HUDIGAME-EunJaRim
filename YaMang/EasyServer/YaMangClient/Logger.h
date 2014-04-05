#pragma once

#include "MacroSet.h"

#ifndef _PRINT_CONSOLE
#define _PRINT_CONSOLE
#endif

class Logger;
typedef Logger	super;

class Logger: public Singleton<Logger>
{
public:
	Logger();
	~Logger();
};

