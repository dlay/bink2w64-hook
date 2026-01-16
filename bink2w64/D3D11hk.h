#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "utils.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "menu.h"
#include "WICTextureLoader.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define PI (3.14159265359f)
#define DEG2RAD(d) ((d)*PI/180.f)
#define safe_release(p) if (p) { p->Release(); p = nullptr; }

long __stdcall hkPresent(IDXGISwapChain * swapChain, UINT SyncInterval, UINT Flags);

class D3D11hk
{
public:
	static D3D11hk& get()
	{
		static D3D11hk _instance;
		return _instance;
	}
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		D3DCOLORVALUE color;
	};

	boolean init = false;

	HWND gameWindow = nullptr;
	uint64_t* g_methodsTable = nullptr;

	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pRenderTargetView = nullptr;
	/* ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11InputLayout* pVertexLayout = nullptr;
	ID3D11PixelShader* pPixelShader = nullptr;
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	ID3D11Buffer* pConstantBuffer = nullptr;
	D3D11_VIEWPORT pViewport; */

	vec3 camPos;
	uintptr_t cameraStruct = 0;
	float tanFovX = 0;
	float tanFovY = 0;
	float halfWidth = 0;
	float halfHeight = 0;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	ImDrawList* drawList = nullptr;
	float dpiScale = 1.0f;

	ID3D11ShaderResourceView* pTexture = nullptr;
	ID3D11ShaderResourceView* pFishTexture = nullptr;
	// std::unique_ptr<DirectX::SpriteBatch> pSpriteBatch = nullptr;

	typedef long(__stdcall* Present)(IDXGISwapChain*, UINT, UINT);
	using fnPresent = HRESULT(__stdcall*)(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags);
	Present oPresent = nullptr;
	void* oPresentAddr;


	bool initHk();
	bool initd3(IDXGISwapChain* swapChain);
	void resize(D3D11_VIEWPORT newViewport);
	bool closeHk();
	void draw();
	void render();

	void initCamera(uintptr_t camAddr);
	void setCameraPos();
	void drawMarker(vec3 markerPos, int colorId);

private:
	D3D11hk() {}
	D3D11hk(const D3D11hk&);
	D3D11hk & operator = (const D3D11hk &);
};

