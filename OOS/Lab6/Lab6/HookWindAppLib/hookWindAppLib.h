#ifndef HOOK_WIND_APP_LIB_H
#define HOOK_WIND_APP_LIB_H

#ifdef HOOK_WIND_APP_LIB_EXPORTS
#define HOOK_WIND_APP_LIB_API __declspec(dllexport)
#else
#define HOOK_WIND_APP_LIB_API __declspec(dllimport)
#endif

HOOK_WIND_APP_LIB_API extern int stopFlag;
HOOK_WIND_APP_LIB_API LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam);
	
#endif