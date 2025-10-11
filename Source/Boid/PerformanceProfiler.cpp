#include "PerformanceProfiler.h"
/*

Big NOTE: This is all static and is basically just C code, but it does not touch any engine state and uses Epic's UNreal wrappers. Therefore, it is ENTIRELY SAFE to call!!!!
Note to self: Do NOT make this touch any engine state - it will end VERY, VERY POORLY!!!

Basically this just asks the CPU "How much time have we wasted this time"? 

Another big problem: This code is *NON-PORTABLE!* 
Sorry if you use linux, I was bored. You can use getticks_ns though (this is left as an exercise to you :D) 
*/

Windows::LARGE_INTEGER cycles; 

void GameProfiler_Init()
{ 
	Windows::QueryPerformanceCounter(&cycles);
}

float GameProfiler_GetMilliseconds()
{
	Windows::LARGE_INTEGER now;

	Windows::QueryPerformanceCounter(&now);
	
	return (float)(1000.0f * now.QuadPart) / cycles.QuadPart;

}