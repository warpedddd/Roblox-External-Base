#include "Globals.hpp"
#include <d3d11.h>
#include "../../backend/core/roblox-development-kit/sdk.hpp"

namespace Globals
{
	namespace Roblox // Roblox Engine Type shit
	{
		Matrix4 ViewMatrix;
		Vectors::Vector2 ViewPortSize;
		Visualengine VisualEngine;

		GameInstance Datamodel;
		GameInstance Players;
		GameInstance Workspace;

		GameInstance LocalPlayer;
	}

	namespace Features
	{
		bool ESPEnabled = false;
	}

	namespace Overlay
	{
		HWND WindowHandle;
		bool VSync;

		ID3D11Device* d3d11_device = nullptr;
		ID3D11DeviceContext* d3d11_device_context = nullptr;
		IDXGISwapChain* dxgi_swap_chain = nullptr;
		ID3D11RenderTargetView* d3d11_render_target_view = nullptr;
	}

	namespace Fonts
	{
		ImFont* inter_bold = nullptr;
	}

	namespace Debug
	{
		bool Pointers = true;
	}
}
