#pragma once
#include "dllexports.h"
#include <Windows.h>
#include <thread>
#include "D3D11hk.h"
#include "hooks.h"

void D3D11hk::draw()
{
	if (state::render)
	{
		setCameraPos();
		for (const std::pair<QWORD, Game::Actor>& pair : Game::Actor::actorMap) {
			if (canShowMarker(pair.second))
			{
				if ((int)pair.second.playerClass == -1 && (int)pair.second.level == 1 && (int)pair.second.race == 0)
				{
					D3D11hk::get().drawMarker(pair.second.pos, 15);
					std::cout << pair.first << std::endl;
				}
				else
				{
					D3D11hk::get().drawMarker(pair.second.pos, (int)pair.second.race);
				}
			}
		}
	}
}

void __stdcall OnAttach(HMODULE hModule)
{
    initIO();
	log("Wait for Game to start...");
	while ((D3D11hk::get().gameWindow = FindWindowA("VVideoClass", nullptr)) == NULL); // window ist schon da wenn solo schriftzug da ist...
	log("Game found.");
	std::this_thread::sleep_for(std::chrono::milliseconds(20000)); // TODO: anderen weg finden
	log("Game Window found.");

	if (D3D11hk::get().initHk())
	{
		log("d3 hook created.");
		initHooks((uintptr_t)GetModuleHandle(L"gujianol.exe"));
		log("game hooks created.");
		fillServerEvents();

		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			if (GetAsyncKeyState(hotkeys::uninject) & 0x1)
				break;
		}
	}
	else
	{
		log("Hook failed.");
	}

	log("teardown.");
	D3D11hk::get().closeHk();
	teardownIO();
	// FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	WCHAR exePath[MAX_PATH + 1];
	DWORD len = GetModuleFileNameW(NULL, exePath, MAX_PATH);
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		if (len > 0) {
			char ch[260];
			char DefChar = ' ';
			WideCharToMultiByte(CP_ACP, 0, exePath, -1, ch, 260, &DefChar, NULL);
			std::string ss(ch);
			// std::cout << "Exe path: " << ss << std::endl;
			std::string gujian = "gujianol.exe";
			std::size_t found = ss.find(gujian);
			if (found != std::string::npos)
			{
				CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)OnAttach, hModule, 0, nullptr);
			}
		}
		else {
			std::cout
				<< L"Error getting exe path: "
				<< GetLastError()
				<< std::endl;
		}
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

