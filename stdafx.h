// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>
#include <stdio.h>
#include <malloc.h>
#include <dbt.h>
#include "csf_accessory_AccessoryCallControl.h"
#include "csf_accessory_AccessoryCommon.h"

const int HAND_SHAKE = 0x6666;
const int BREAK_UP = 0x7777;

enum CallState
{
	Free    = 0x00,
	Ringer  = 0x01,
	OffHook = 0x02,
	Mute    = 0x03,
	Flash   = 0x04
};

void SendEventToIntegration(int event);
DWORD WINAPI CommucationThreadProc(LPVOID lpParam);
// TODO: reference additional headers your program requires here
