#pragma once
#include "global.h"

class DebugTimer{
	LONGLONG first, end, time;
	char* name;
public:
	DebugTimer();
	DebugTimer(char* name);
	void Stop();
	void Draw(int x, int y) const;
};