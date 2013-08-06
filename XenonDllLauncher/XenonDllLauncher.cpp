// XenonDllLauncher.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

extern "C" 
{ 
	DWORD	ExCreateThread(PHANDLE pHandle, DWORD dwStackSize, LPDWORD lpThreadId, 
							VOID*	apiThreadStartup, LPTHREAD_START_ROUTINE lpStartAddress, 
	LPVOID	lpParameter, DWORD dwCreationFlagsMod);
	VOID	XapiThreadStartup(VOID (__cdecl *StartRoutine)(VOID*), VOID* StartContext);
	DWORD	XexLoadImage(CHAR* ImagePath, DWORD LoadFlags, DWORD Version, HMODULE* ModuleHandle);
}

DWORD	DllResult = -1;
char	DllPath[] = "game:\\XenonDll.xex";
char	GameXexPath[] = "game:\\default.xex";

void StartSystemDll( void )
{
	DllResult = XexLoadImage((CHAR*)&DllPath, 8, NULL, NULL);
}

//-------------------------------------------------------------------------------------
// Name: main()
// Desc: The application's entry point
//-------------------------------------------------------------------------------------
void __cdecl main()
{
     // Create a system thread to launch our "system plugin"
	HANDLE	hThread;
	DWORD	threadId;
	ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)StartSystemDll, NULL, 0x2);

	// Set the thread processor and priority, otherwise it will start on its own
	XSetThreadProcessor(hThread, 4);
	SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
	ResumeThread(hThread);

	// Just loop until the dll has been loaded
	while (DllResult == -1) { Sleep(0); }

	// The dll has loaded, now launch the game executable
	XLaunchNewImage((CHAR*)&GameXexPath, NULL);
}

