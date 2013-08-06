#include "stdafx.h"
#include "SysDllMain.h"
#include <stdio.h>			// Required for printf functions
#include "SystemFunctions.h"// Kernel/XAM imports + other related functions
#include <xinput2.h>
#include <ctime>
#include <time.h>
#include <xbdm.h>

// Easier way of working with DMGetMemory and DmSetMemory (Thanks Lander ;))
#define SetMemory(Dest, Source, Size) { DmSetMemory(Dest, Size, Source, NULL); }
#define GetMemory(Source, Destination, Size) { DmGetMemory(Source, Size, Destination, NULL); }

void DebugPrint(const char* format, ...)
{
	va_list pArgList;
    va_start( pArgList, format );

    CHAR buffer[ 1000 ];
    vsprintf( buffer, format, pArgList );

    printf("[%s]: %s\n", MODULE_NAME, buffer );
}

void GetControllerInput( void )
{
	DWORD dwResult;    
	for (DWORD i=0; i< 3; i++ )
	{
		XINPUT_STATE state;
		ZeroMemory( &state, sizeof(XINPUT_STATE) );

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState( i, &state );

		if( dwResult == ERROR_SUCCESS )
		{ 
			// Controller is connected
			std::vector<DWORD> Buttons;
			DWORD wButtons = state.Gamepad.wButtons;
			if (wButtons & XINPUT_GAMEPAD_A)
				Buttons.push_back(ControllerButtons::A);
			if (wButtons & XINPUT_GAMEPAD_B)
				Buttons.push_back(ControllerButtons::B);
			if (wButtons & XINPUT_GAMEPAD_X)
				Buttons.push_back(ControllerButtons::X);
			if (wButtons & XINPUT_GAMEPAD_Y)
				Buttons.push_back(ControllerButtons::Y);
			if (wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
				Buttons.push_back(ControllerButtons::RIGHT_BUMPER);
			if (wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
				Buttons.push_back(ControllerButtons::LEFT_BUMPER);
			if (wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
				Buttons.push_back(ControllerButtons::RIGHT_BUMPER);
			if (wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
				Buttons.push_back(ControllerButtons::LEFT_THUMBSTICK);
			if (wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
				Buttons.push_back(ControllerButtons::RIGHT_THUMBSTICK);
			if (wButtons & XINPUT_GAMEPAD_DPAD_UP)
				Buttons.push_back(ControllerButtons::DPAD_UP);
			if (wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				Buttons.push_back(ControllerButtons::DPAD_DOWN);
			if (wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
				Buttons.push_back(ControllerButtons::DPAD_LEFT);
			if (wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
				Buttons.push_back(ControllerButtons::DPAD_RIGHT);
			if (wButtons & XINPUT_GAMEPAD_START)
				Buttons.push_back(ControllerButtons::START);
			if (wButtons & XINPUT_GAMEPAD_BACK)
				Buttons.push_back(ControllerButtons::BACK);

			// Now that we've gotten all of the button inputs, let's see if this combination matches any
			// of the ones we defined
			for (int i = 0; i < ControllerInputActions.size(); i++)
			{
				InputResponse ir = ControllerInputActions[i];
				if (ir.ButtonPresses.size() != Buttons.size())
					continue;

				bool ValidInput = true;
				for (int j = 0; j < ir.ButtonPresses.size(); j++)
				{
					bool Found = false;
					for (int k = 0; k < Buttons.size(); k++)
					{
						if (ir.ButtonPresses[j] == Buttons[k])
						{
							Found = true;
							break;
						}
					}
					if (!Found)
					{
						ValidInput = false;
						break;
					}
				}

				// If we got a matching controller input
				if (ValidInput)
				{
					ir.Action();
					break;
				}
			}
		}
		else
		{
			// Controller is not connected 
		}
	}
}

void InitialXexSetup( void )
{
	// Alex's xbox is retarded, use this to nop a fucking breakpoint in the kernal. I mean fucking hell microsoft.
	byte nopOpcode[4] = { 0x60, 0x00, 0x00, 0x00 };
	SetMemory((void*)0x81B0C8B0, (void*)&nopOpcode, sizeof(nopOpcode));
}

// Add code you want to be executed during every update while in the game
void Update( void )
{
	GetControllerInput();
}

void GameCheck( void )
{
	// Setup Button Combo's

	// Set up a label so we can jump back to this code if we leave the title id
Start:
	while (XamGetCurrentTitleId() != GAME_TITLE_ID) { Sleep(0); }
	DebugPrint("Game has been launched");

	// Poke initial bytes into the xex in Memory
	InitialXexSetup();

	// We have entered the game, do our update code
	while (XamGetCurrentTitleId() == GAME_TITLE_ID) { Update(); }

	// The loop broke, which means they aren't on the game anymore
	DebugPrint("Game has been exited");
	goto Start;
}

// Enter SystemDllMain.h and change the game title ID before making any other edits!
DWORD Initialize( void )
{
	//if (CreateSymbolicLink("hdd1:", CONTENT_PARTITION_PATH, true) != S_OK)
	//	return S_FALSE;

	//Create our new thread to wait on halo for. 
	DWORD threadId;
	ExCreateThread(&ModuleHandle, 0, &threadId, (VOID*)GameCheck, (LPTHREAD_START_ROUTINE)GameCheck, NULL, 0x02);

	return S_OK;
}

BOOL APIENTRY DllMain(HANDLE hInstDLL, DWORD reason, LPVOID lpReserved)
{
	ModuleHandle = hInstDLL;
	switch(reason)
	{
		case DLL_PROCESS_ATTACH:
			if(Initialize() != S_OK)
				return false;
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
	return true;
}
