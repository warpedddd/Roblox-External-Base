#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <cmath>
#include <Windows.h>
#include <d3d11.h>
#include "../../frontend/dependencies/user-interface/imgui.h"
#include "../../backend/core/roblox-development-kit/sdk.hpp"

namespace Globals
{
	namespace Roblox // Roblox Engine Type shit
	{
		extern Matrix4 ViewMatrix;
		extern Vectors::Vector2 ViewPortSize;
		extern Visualengine VisualEngine;

		extern GameInstance Datamodel;
		extern GameInstance Players;
		extern GameInstance Workspace;

		extern GameInstance LocalPlayer;
	}

	namespace Features
	{
		extern bool ESPEnabled;
	}

	namespace Overlay
	{
		extern bool VSync;
		extern HWND WindowHandle;

		extern ID3D11Device* d3d11_device;
		extern ID3D11DeviceContext* d3d11_device_context;
		extern IDXGISwapChain* dxgi_swap_chain;
		extern ID3D11RenderTargetView* d3d11_render_target_view;
	}

	namespace Fonts
	{
		extern ImFont* inter_bold;
	}

	namespace Debug
	{
		extern bool Pointers;
	}
}

#endif // GLOBALS_HPP
