#include "Timer.h"

Timer::Timer()
{}

Timer::~Timer()
{}

uint64_t Timer::GetLocalTime()
{
	time_t tt;
	time(&tt);
	struct tm *time = localtime(&tt);
	uint64_t ts = mktime(time);
	return ts;
}
