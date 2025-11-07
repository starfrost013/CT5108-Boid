#pragma once
#include <Microsoft/MinimalWindowsApi.h>
#if PLATFORM_WINDOWS
#include <windows.h>
#endif

void GameProfiler_Init();
float GameProfiler_GetMilliseconds();