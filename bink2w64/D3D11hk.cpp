#include "pch.h"
#include "D3D11hk.h"

bool D3D11hk::initHk()
{
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	DXGI_RATIONAL refreshRate;
	refreshRate.Numerator = 60;
	refreshRate.Denominator = 1;

	DXGI_MODE_DESC bufferDesc;
	bufferDesc.Width = 100;
	bufferDesc.Height = 100;
	bufferDesc.RefreshRate = refreshRate;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SAMPLE_DESC sampleDesc;
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = gameWindow;
	swapChainDesc.Windowed = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context);

	void** pVMT = *(void***)swapChain;

	// Get Present's address out of vmt
	oPresentAddr = (fnPresent)(pVMT[8]);

	/*g_methodsTable = (uint64_t*)::calloc(205, sizeof(uint64_t));
	::memcpy(g_methodsTable, *(uint64_t**)swapChain, 18 * sizeof(uint64_t));
	::memcpy(g_methodsTable + 18, *(uint64_t**)device, 43 * sizeof(uint64_t));
	::memcpy(g_methodsTable + 18 + 43, *(uint64_t**)context, 144 * sizeof(uint64_t));

	if (MH_Initialize() != MH_OK)
	{
		log("MinHook initialization failed.");
		return false;
	}*/

	// swapChain->Release();
	// swapChain = NULL;

	device->Release();
	device = NULL;

	context->Release();
	context = NULL;

	/*void* target = (void*)g_methodsTable[8];
	if (MH_CreateHook(target, reinterpret_cast<LPVOID*>(hkPresent), (void**)&oPresent) != MH_OK || MH_EnableHook(target) != MH_OK)
	{
		log("binding failed.");
		return false;
	}*/

	return true;
}

void D3D11hk::initCamera(uintptr_t camAddr)
{
	cameraStruct = camAddr;
}

void D3D11hk::setCameraPos()
{
	float* cameraPositionX = (float*)(cameraStruct + 0x04);
	float* cameraPositionY = (float*)(cameraStruct + 0x08);
	float* cameraPositionZ = (float*)(cameraStruct + 0x0C);
	camPos = { (*cameraPositionX) ,(*cameraPositionY) ,(*cameraPositionZ) };

	tanFovX = tanf(DEG2RAD(81.5560f) / 2.0f); // TODO: find in memory off of base pointer
	tanFovY = tanf(DEG2RAD(50.8499f) / 2.0f);
	ImVec2 size = ImGui::GetMainViewport()->Size;
	halfWidth = size.x / 2.0f;
	halfHeight = size.y / 2.0f;
}

void D3D11hk::drawMarker(vec3 markerPos, int colorId)
{
	if (cameraStruct == 0)
		return;

	vec3 result = { markerPos.x - camPos.x,  markerPos.y - camPos.y,  markerPos.z - camPos.z };

	float dotForward = *(float*)(cameraStruct + 0x0138) * result.x + *(float*)(cameraStruct + 0x0148) * result.y + *(float*)(cameraStruct + 0x0158) * result.z;
	float dotRight = *(float*)(cameraStruct + 0x0130) * result.x + *(float*)(cameraStruct + 0x0140) * result.y + *(float*)(cameraStruct + 0x0150) * result.z;
	float dotLeft = dotRight * -1;
	float dotUp = *(float*)(cameraStruct + 0x0134) * result.x + *(float*)(cameraStruct + 0x0144) * result.y + *(float*)(cameraStruct + 0x0154) * result.z;

	vec3 screen;
	// if (dotForward > 0.05f) {
	if (dotForward < 0) {
		dotForward *= -1;
	}
	if (dotForward > 0.05f || dotForward < -0.05f) {
		screen.x = halfWidth - dotLeft * halfWidth / (dotForward * tanFovX);
		screen.y = halfHeight - dotUp * halfHeight / (dotForward * tanFovY);
		screen.z = sqrtf(result.x * result.x + result.y * result.y + result.z * result.z); // dist from camera
	}
	// if (screen.z > 500.f)
		// fprintf(hf_out, "dist: %f\n", (100000 / screen.z));

	// TODO: scale triange size with screen.z
	vec4 color = getDistinctColor(colorId);
	if (colorId == 15)
	{
		drawList->AddImage((void*)pTexture, ImVec2(screen.x - 24, screen.y - 24), ImVec2(screen.x + 24, screen.y + 24));
		drawList->AddLine(ImVec2(screen.x, screen.y), ImVec2(halfWidth, halfHeight), IM_COL32(color.x, color.y, color.z, color.w));
	}
	else if (colorId == 8)
	{
		drawList->AddImage((void*)pFishTexture, ImVec2(screen.x - 24, screen.y - 24), ImVec2(screen.x + 24, screen.y + 24));
	}
	else
	{
		drawList->AddTriangleFilled(ImVec2(screen.x - 20, screen.y - 66), ImVec2(screen.x + 20, screen.y - 66), ImVec2(screen.x, screen.y), IM_COL32(color.x, color.y, color.z, color.w));
	}
}

bool D3D11hk::initd3(IDXGISwapChain* swapChain)
{
	init = true;
	pSwapChain = swapChain;
	pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);
	if (!pDevice) {
		log("no device");
		return false;
	}
	log("got device.");
	pDevice->GetImmediateContext(&pContext);
	if (!pContext) {
		log("no context");
		return false;
	}
	log("got context.");
	DXGI_SWAP_CHAIN_DESC pSwapDesc;
	pSwapChain->GetDesc(&pSwapDesc);
	gameWindow = pSwapDesc.OutputWindow;
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = "Interface/Addons/soloUI/gui.ini";
	io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 5.f;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(gameWindow);
	ImGui_ImplDX11_Init(pDevice, pContext);
	ImGui::GetIO().ImeWindowHandle = gameWindow;
	/*ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	pBackBuffer->Release();*/

	DirectX::CreateWICTextureFromFile(pDevice, L"Interface/Addons/soloUI/gather_icon.png", nullptr, &pTexture);
	DirectX::CreateWICTextureFromFile(pDevice, L"Interface/Addons/soloUI/fish_icon.png", nullptr, &pFishTexture);

	ImGui_ImplWin32_EnableDpiAwareness();
	dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(gameWindow);
	io.Fonts->AddFontFromFileTTF("Interface/Addons/soloUI/Roboto-Medium.ttf", 14.0f * dpiScale);
	// io.FontGlobalScale = dpiScale;
	ImGui::GetStyle().ScaleAllSizes(dpiScale);
	log("imgui init done.");
	return true;
}

void D3D11hk::resize(D3D11_VIEWPORT newViewport)
{
	/*pViewport = newViewport;
	DirectX::XMMATRIX mOrtho = DirectX::XMMatrixOrthographicOffCenterLH(0, pViewport.Width, pViewport.Height, 0, 0.0f, 1.0f);
	D3D11_BUFFER_DESC bufferDesc{ 0 };
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = sizeof(mOrtho);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subresourceData{ 0 };
	subresourceData.pSysMem = &mOrtho;
	HRESULT hr = pDevice->CreateBuffer(&bufferDesc, &subresourceData, &pConstantBuffer);
	halfWidth = pViewport.Width / 2.0f;
	halfHeight = pViewport.Height / 2.0f;
	pContext->RSSetViewports(1, &pViewport);*/
}

bool D3D11hk::closeHk()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	// ImGui::DestroyContext();
	safe_release(pDevice);
	safe_release(pSwapChain);
	safe_release(pContext);
	safe_release(pRenderTargetView);
	return true;
}

void D3D11hk::render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	drawList = ImGui::GetBackgroundDrawList();

	draw();

	if (state::showEventTimer)
	{
		drawEventTimer();
	}

	if (state::renderDebug)
	{
		ImGui::ShowDemoWindow();
		drawList->AddRect(ImVec2(50, 50), ImVec2(150, 150), IM_COL32(255, 255, 0, 255));
		drawList->AddImage((void*)pTexture, ImVec2(0, 0), ImVec2(48, 48));
	}

	if (state::showMenu)
	{
		drawMarkerMenu();
		drawRecordingMenu();
	}

	ImGui::EndFrame();
	ImGui::Render();

	// pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
