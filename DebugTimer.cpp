#include "DebugTimer.h"

DebugTimer::DebugTimer() {
	first = GetNowHiPerformanceCount();
	end = 0;
	name = "None";
}

DebugTimer::DebugTimer(char* dname) {
	first = GetNowHiPerformanceCount();
	end = 0;
	name = dname;
}

void DebugTimer::Stop() {
	end = GetNowHiPerformanceCount();
	time = end - first;
}

void DebugTimer::Draw(int x, int y) const{
	if(time > 8000) DrawFormatString(x, y, RED, "%s : %.2f ms", name, time / 1000.0);
	else DrawFormatString(x, y, WHITE, "%s : %.2f ms", name, time/1000.0);
}