// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "SendData.h"

using namespace std;

bool inGame;
int startTimer = 0; // wait a couple ticks before setting inGame flag

// Player data addresses
short *localClient = (short*)0x402AAFCE;
char *playerName = (char*)0x402AAFD0;
short *kills = (short*)0x402AB068;
short *deaths = (short*)0x402AB07A;
short *assists = (short*)0x402AB070;

// Other game variables
// bool *DevModeSwitch = (bool*)0x006BD17E;
char *currentMap = (char*)0x00643064;

PlayerData player;

DWORD WINAPI MainLoop(LPVOID)
{

	HANDLE halo = GetCurrentProcess();

	inGame = false;

	while (true)
	{
		// If Player is in Main Menu
		if (currentMap[0] == 'u' && currentMap[1] == 'i') {
			// If player was in a game and left
			if (inGame) {
				// Can probably move stat recording here if they aren't overwritten
				SendData(formatJson(player));
				// Restting player data addresses
				localClient = (short*)0x402AAFCE;
				playerName = (char*)0x402AAFD0;
				kills = (short*)0x402AB068;
				deaths = (short*)0x402AB07A;
				assists = (short*)0x402AB070;
			}
			inGame = false;
		}

		// If player is in a match
		else if (startTimer > 5) {
			inGame = true;
			// Get the right player
			while (*localClient != 0) {
				playerName += 0x200;
				kills += 0x100; // Cause short is 2 bytes
				deaths += 0x100;
				assists += 0x100;
				localClient += 0x100;
			}
			player.name = getPlayerName(halo, playerName);
			ReadProcessMemory(halo, (void*)(kills), &player.kills, sizeof(player.kills), 0);
			ReadProcessMemory(halo, (void*)(deaths), &player.deaths, sizeof(player.deaths), 0);
			ReadProcessMemory(halo, (void*)(assists), &player.assists, sizeof(player.assists), 0);
		}

		// For testing
		//if (GetAsyncKeyState(VK_NEXT) & 1) // PageDown, once
		//{
		//	//*DevModeSwitch = 0; // disable devmode
		//	*deaths = 0;
		//}
		//else if (GetAsyncKeyState(VK_PRIOR) & 1) // PageUp, once
		//{
		//	//*DevModeSwitch = 1; // enable devmode
		//	//*deaths = 10;
		//	WriteProcessMemory(halo, deaths, &deathTest, 2, 0); 
		//}

		startTimer++;

		Sleep(1000); // 1 sec
	}
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ulReason, LPVOID lpReserved)
{
	if (ulReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, MainLoop, 0, 0, 0); // create a new thread for the app so injection won't fail
	}
	return TRUE;
}