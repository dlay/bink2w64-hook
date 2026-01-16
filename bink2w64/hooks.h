#pragma once
#include "utils.h"
#include "D3D11hk.h"
#include "actor.h"

typedef signed __int64(__fastcall* allActors)(QWORD, QWORD, QWORD*);
extern allActors oActors;

typedef char(__fastcall* allCameras)(__int64 a1, __int64 a2, OWORD* a3, __int64 a4, __int64 a5, float a6, float a7, unsigned __int8 a8);
extern allCameras oCameras;


void initHooks(uintptr_t moduleBaseAddr);
signed __int64 __fastcall actorsHk(QWORD _rcx, QWORD _rdx, QWORD* _r8);
char __fastcall camerasHk(__int64 a1, __int64 a2, OWORD* a3, __int64 a4, __int64 a5, float a6, float a7, unsigned __int8 a8);
long __stdcall presentHk(IDXGISwapChain* swapChain, UINT SyncInterval, UINT Flags);
LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void testHk();
