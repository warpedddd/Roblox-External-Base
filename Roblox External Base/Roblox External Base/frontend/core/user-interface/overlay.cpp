#include "overlay.hpp"
#include <Windows.h>
#include <dwmapi.h>

#include <d3d11.h>
#include <dxgi.h>
#include <iostream>
#include "../../dependencies/user-interface/imgui_impl_win32.h"
#include "../../../utilities/globals/globals.hpp"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

bool CreateDevice(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 144;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	const UINT create_device_flags = 0;
	D3D_FEATURE_LEVEL d3d_feature_level;
	const D3D_FEATURE_LEVEL feature_level_arr[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, create_device_flags, feature_level_arr, 2, D3D11_SDK_VERSION, &sd, &Globals::Overlay::dxgi_swap_chain, &Globals::Overlay::d3d11_device, &d3d_feature_level, &Globals::Overlay::d3d11_device_context);
	if (res == DXGI_ERROR_UNSUPPORTED)
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, create_device_flags, feature_level_arr, 2, D3D11_SDK_VERSION, &sd, &Globals::Overlay::dxgi_swap_chain, &Globals::Overlay::d3d11_device, &d3d_feature_level, &Globals::Overlay::d3d11_device_context);
	if (res != S_OK)
		return false;

    Globals::Overlay::d3d11_device_context->OMSetRenderTargets(1, &Globals::Overlay::d3d11_render_target_view, nullptr);

    return true;
}


void CreateTarget() {
	ID3D11Texture2D* d3d11_back_buffer;
	Globals::Overlay::dxgi_swap_chain->GetBuffer(0, IID_PPV_ARGS(&d3d11_back_buffer));
	if (d3d11_back_buffer != nullptr)
	{
		Globals::Overlay::d3d11_device->CreateRenderTargetView(d3d11_back_buffer, nullptr, &Globals::Overlay::d3d11_render_target_view);
		d3d11_back_buffer->Release();
	}
}

void CleanupTarget() {
	if (Globals::Overlay::d3d11_render_target_view) {
		Globals::Overlay::d3d11_render_target_view->Release();
		Globals::Overlay::d3d11_render_target_view = nullptr;
	}
}

void CleanupDevice() {
	CleanupTarget();

	if (Globals::Overlay::dxgi_swap_chain) { Globals::Overlay::dxgi_swap_chain->Release(); Globals::Overlay::dxgi_swap_chain = nullptr; }
	if (Globals::Overlay::d3d11_device_context) { Globals::Overlay::d3d11_device_context->Release(); Globals::Overlay::d3d11_device_context = nullptr; }
	if (Globals::Overlay::d3d11_device) { Globals::Overlay::d3d11_device->Release(); Globals::Overlay::d3d11_device = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (Globals::Overlay::d3d11_device != nullptr && wParam != SIZE_MINIMIZED)
		{
			CleanupTarget();
			Globals::Overlay::dxgi_swap_chain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateTarget();
		}
		return 0;

	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool Overlay::InitDX11Overlay()
{
	WNDCLASSEX wc{};
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = TEXT("Roblox External Base");
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&wc);
	const HWND hw = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, wc.lpszClassName, TEXT("Roblox External Base"), WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);

	SetLayeredWindowAttributes(hw, 0, 255, LWA_ALPHA);
	const MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(hw, &margin);

	if (!CreateDevice(hw))
	{
		CleanupDevice();
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return false;
	}

	ShowWindow(hw, SW_SHOW);
	UpdateWindow(hw);

    Globals::Overlay::WindowHandle = hw;
}
