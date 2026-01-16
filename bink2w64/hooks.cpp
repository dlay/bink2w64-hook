#include "hooks.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
allActors oActors = nullptr;
allCameras oCameras = nullptr;
int runtime = 0;
WNDPROC	oWndProc;

void initHooks(uintptr_t pModuleBaseAddr)
{
	// oCameras = (allCameras)DetourFunction64(((void*)(pModuleBaseAddr + 0x90b140)), camerasHk, 18);
	oCameras = (allCameras)DetourFunction64(((void*)(pModuleBaseAddr + 0x8de2d0)), camerasHk, 18);
	// oActors = (allActors)DetourFunction64(((void*)(pModuleBaseAddr + 0x1b5e10)), actorsHk, 18);
	oActors = (allActors)DetourFunction64(((void*)(pModuleBaseAddr + 0x17dab0)), actorsHk, 18);
	D3D11hk::get().oPresent = (D3D11hk::Present)DetourFunction64(D3D11hk::get().oPresentAddr, presentHk, 14);
}

LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	if (state::showMenu)
	{
		POINT mPos;
		GetCursorPos(&mPos);
		ScreenToClient(D3D11hk::get().gameWindow, &mPos);
		io.MousePos.x = (float)mPos.x;
		io.MousePos.y = (float)mPos.y;
	}


	if (uMsg == WM_KEYUP)
	{
		if (wParam == hotkeys::openMenu)
			state::showMenu = !state::showMenu;

		if (wParam == hotkeys::render)
			state::render = !state::render;

		/*if (wParam == hotkeys::shutdown)
			state::shutdown = !state::shutdown;

		if (wParam == hotkeys::hideAll)
			state::hideAll = !state::hideAll;

		if (wParam == hotkeys::showEventTimer)
			state::showEventTimer = !state::showEventTimer;

		if (wParam == hotkeys::scaleUiSmall)
		{
			ImGui::GetStyle().ScaleAllSizes(0.5f);
			D3D11hk::get().dpiScale /= 2.0f;
			// io.Fonts->AddFontFromFileTTF("Interface/Addons/soloUI/Roboto-Medium.ttf", 13.0f * D3D11hk::get().dpiScale);
			io.FontGlobalScale = D3D11hk::get().dpiScale;
		}

		if (wParam == hotkeys::scaleUiBig)
		{
			ImGui::GetStyle().ScaleAllSizes(2.0f);
			D3D11hk::get().dpiScale *= 2.0f;
			// io.Fonts->AddFontFromFileTTF("Interface/Addons/solUI/Roboto-Medium.ttf", 26.0f);
			io.FontGlobalScale = D3D11hk::get().dpiScale;
		}

		if (wParam == hotkeys::renderDebug)
			state::renderDebug = !state::renderDebug;*/
	}

	if (uMsg == WM_SIZE)
	{
		// resize
	}

	if (state::showMenu)
	{
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		return true;
	}
	return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
}

signed __int64 __fastcall actorsHk(QWORD _rcx, QWORD _rdx, QWORD* _r8)
{
	if (state::shutdown) return oActors(_rcx, _rdx, _r8);
	/*if (!gotCharCoords) {
		uintptr_t actorStructAddr = followPointerPath(moduleBaseAddr + 0x304FE60, { 0x40,0x120,0x260,0x28,0x10 });
		charPositionX = (float*)(followPointerPath(actorStructAddr, { 0xB0 }));
		charPositionY = (float*)(followPointerPath(actorStructAddr, { 0xB4 }));
		charPositionZ = (float*)(followPointerPath(actorStructAddr, { 0xB8 }));
		gotCharCoords = true;
	}
	QWORD rcx = _rcx;
	QWORD rdx = _rdx;
	QWORD* r8 = _r8;
	OWORD2FLOAT camCoords = *(OWORD2FLOAT*)(_rcx + 0xb0);
	if (!emptyDicts) {
		OWORD2FLOAT camCoords = *(OWORD2FLOAT*)(_rcx + 0xb0);
		tempDict1[_rcx] = rcx;
		tempDict2[_rcx] = rdx;
		tempDict3[_rcx] = *r8;
		tempDict4[_rcx] = *(QWORDPTR*)r8;
		tempDict5[_rcx] = *(int*)(_rcx + 0x158);
		differentCameraBases[_rcx] = camCoords;
	}
	if (emptyDictsCounter++ > emptyDictsThreashold)
		emptyDicts = true;

	//differentCameraBases[_rcx] = camCoords;*/

	if (runtime > 100000 && !state::recordingMarker)
		Game::Actor::resetActorList();

	if (runtime > 100000)
		runtime = 0;

	OWORD2FLOAT camCoords = *(OWORD2FLOAT*)(_rcx + 0xb0);
	int level = *(int*)(_rcx + 0x130);
	int playerClass = *(int*)(_rcx + 0x58);
	int race = *(int*)(_rcx + 0x158);

	int idk1 = *(int*)(_rcx + 0x128);
	int idk2 = *(int*)(_rcx + 0x138);
	int idk3 = *(int*)(_rcx + 0x168);
	int idk4 = *(int*)(_rcx + 0x70);
	int idk5 = *(int*)(_rcx + 0x178);

	Game::Actor newActor({ camCoords.x, camCoords.y, camCoords.z }, level, playerClass, race);
	newActor.debugVal1 = idk1;
	newActor.debugVal2 = idk2;
	newActor.debugVal3 = idk3;
	newActor.debugVal4 = idk4;
	newActor.debugVal5 = idk5;

	Game::Actor::addActor(_rcx, newActor);

	runtime++;
	return oActors(_rcx, _rdx, _r8);
}

void testHk()
{
	if (runtime > 100000)
	{
		Game::Actor::resetActorList();
		runtime = 0;
	}

	Game::Actor newActor({ 1234.34f, 3232.55f, 6547.36f }, 22, 2, 3);
	QWORD _rcx = 123;

	Game::Actor::addActor(_rcx, newActor);

	runtime++;
}

char __fastcall camerasHk(__int64 a1, __int64 a2, OWORD* a3, __int64 a4, __int64 a5, float a6, float a7, unsigned __int8 a8)
{
	if (state::shutdown) return(oCameras(a1, a2, a3, a4, a5, a6, a7, a8));

	if (D3D11hk::get().cameraStruct == 0)
	{
		if ((int)a6 == 5)
		{
			D3D11hk::get().initCamera(a1);
			log("camera hooked.");
		}
	}

	return(oCameras(a1, a2, a3, a4, a5, a6, a7, a8));
}

long __stdcall presentHk(IDXGISwapChain* swapChain, UINT SyncInterval, UINT Flags)
{
	if (!D3D11hk::get().init)
		if (D3D11hk::get().initd3(swapChain))
			oWndProc = (WNDPROC)SetWindowLongPtr(D3D11hk::get().gameWindow, GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProc);

	if (!state::hideAll && !state::shutdown)
		D3D11hk::get().render();

	/*if (isResizing) {
		ID3D11Device* pDevice = nullptr;
		ID3D11DeviceContext* pContext = nullptr;
		pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);
		if (!pDevice) {
			log("no device");
			return d3hk.oPresent(pSwapChain, SyncInterval, Flags);
		}
		pDevice->GetImmediateContext(&pContext);
		if (!pContext) {
			log("no context");
			return d3hk.oPresent(pSwapChain, SyncInterval, Flags);
		}
		UINT numViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
		D3D11_VIEWPORT viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{ 0 };
		pContext->RSGetViewports(&numViewports, viewports);
		if (!viewports[0].Width) {
			log("no viewport");
			return d3hk.oPresent(pSwapChain, SyncInterval, Flags);
		}
		d3hk.resize(viewports[0]);
		std::cout << "Width original: " << viewports[0].Width << ", width hook: " << d3hk.pViewport.Width << std::endl;
		isResizing = false;
		return d3hk.oPresent(pSwapChain, SyncInterval, Flags);
	}*/

	return D3D11hk::get().oPresent(swapChain, SyncInterval, Flags);
}
