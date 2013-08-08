#include "stdafx.h"
#include <vector>
#include <functional>

#define GAME_TITLE_ID 0x4D530919
const char* MODULE_NAME = "ProjectXenon";
const char* CONTENT_PARTITION_PATH = "\\Device\\Harddisk0\\Partition1";

enum ControllerButtons
{
	A,
	B,
	X,
	Y,
	LEFT_THUMBSTICK,
	RIGHT_THUMBSTICK,
	LEFT_BUMPER,
	RIGHT_BUMPER,
	DPAD_UP,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT,
	START,
	BACK
};
typedef struct _inputResponse
{
	std::vector<DWORD> ButtonPresses;
	std::function<void ()> Action;
} InputResponse, *PInputResponse;

HANDLE ModuleHandle;
std::vector<InputResponse> ControllerInputActions;
