#pragma once
#include "stdafx.h"
#include <stdio.h>

typedef struct _ANSI_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PCHAR Buffer;
} ANSI_STRING, *PANSI_STRING;

extern "C"
{
	//-------------------------------------------------------------------------------------
	// Name: XamGetCurrentTileId
	// Desc: Gets the title ID for the current executable
	//-------------------------------------------------------------------------------------
	DWORD			XamGetCurrentTitleId();

	//-------------------------------------------------------------------------------------
	// Name: ObCreateSymbolicLink
	// Desc: Creates a symbolic link for a device path
	//-------------------------------------------------------------------------------------
	DWORD WINAPI	ObCreateSymbolicLink(   PANSI_STRING LinkName,
												PANSI_STRING DevicePath);

	//-------------------------------------------------------------------------------------
	// Name: ObDeleteSymbolicLink
	// Desc: Destroys a linked path
	//-------------------------------------------------------------------------------------
	DWORD WINAPI	ObDeleteSymbolicLink(   PANSI_STRING LinkName);

	DWORD			ExCreateThread(PHANDLE pHandle, DWORD dwStackSize, LPDWORD lpThreadId, 
									VOID*	apiThreadStartup, LPTHREAD_START_ROUTINE lpStartAddress, 
									LPVOID	lpParameter, DWORD dwCreationFlagsMod);
	VOID			XapiThreadStartup(VOID (__cdecl *StartRoutine)(VOID*), VOID* StartContext);
	DWORD			XexLoadImage(CHAR* ImagePath, DWORD LoadFlags, DWORD Version, HMODULE* ModuleHandle);

}

ANSI_STRING CreateAnsiString(const char* String)
{
	char *Buffer = new char[strlen(String) + 1];
	memset(Buffer, 0, strlen(String) + 1);
	sprintf(Buffer, String);
	ANSI_STRING as =
	{
		strlen(String), strlen(String), Buffer
	};
	return as;
}

//-------------------------------------------------------------------------------------
// Name:  CreateSymbolicLink
// Desc:  Mounts a partition, making it ready for reading/writing
// Notes: DevicePath parameter should contain preceding backslashes (e.g. \\Device\\Flash)
// and Link should not have trailing backslashes (e.g. flash:)
//-------------------------------------------------------------------------------------
DWORD CreateSymbolicLink(const char* DevicePath, const char* Link, bool System = false)
{
	char mountName[31];
	if (System)
		sprintf_s( mountName,"\\System??\\%s", Link );
	else
		sprintf_s( mountName,"\\??\\%s", Link );

	ANSI_STRING deviceOriginal =							// Original device path
								{	strlen(DevicePath),		// Length of string
									strlen(DevicePath) + 1,	// Length of string + 1 (null terminator)
									(char*)DevicePath };	// String

	ANSI_STRING deviceMounted =								// Mounted link
								{	strlen(mountName),
									strlen(mountName) + 1,
									mountName };

	return ObCreateSymbolicLink(	&deviceMounted,
									&deviceOriginal );
}

//-------------------------------------------------------------------------------------
// Name: DeleteSymbolicLink
// Desc: Unmounts a linked partition
//-------------------------------------------------------------------------------------
DWORD DeleteSymbolicLink(char* link)
{
	char mountName[31];
	sprintf_s( mountName,"\\??\\%s", link );
	
	ANSI_STRING deviceMounted =								// Mounted link
								{	strlen(mountName),		// Length of string
									strlen(mountName) + 1,	// Length of string + 1 (null terminator)
									mountName };			// String

	return ObDeleteSymbolicLink(&deviceMounted);
}