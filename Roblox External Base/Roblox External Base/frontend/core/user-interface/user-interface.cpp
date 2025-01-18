#include "user-interface.hpp"
#include "../../dependencies/user-interface/imgui.h"
#include "../../dependencies/user-interface/imgui_impl_dx11.h"
#include "../../dependencies/user-interface/imgui_impl_win32.h"

#include <Windows.h>
#include "../../../utilities/globals/globals.hpp"
#include "../../dependencies/user-interface/imgui_internal.h"
#include "fonts.hpp"
#include "../../../backend/core/features/visuals.hpp"


bool UI::Setup() {
	DXGI_SWAP_CHAIN_DESC swap_chain_description = {};
	ZeroMemory(&swap_chain_description, sizeof(swap_chain_description));
	swap_chain_description.BufferCount = 2;
	swap_chain_description.BufferDesc.Width = 0;
	swap_chain_description.BufferDesc.Height = 0;
	swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_description.BufferDesc.RefreshRate.Numerator = Globals::Overlay::VSync ? 144 : 0;
	swap_chain_description.BufferDesc.RefreshRate.Denominator = Globals::Overlay::VSync ? 1 : 0;
	swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.OutputWindow = Globals::Overlay::WindowHandle;
	swap_chain_description.SampleDesc.Count = 1;
	swap_chain_description.SampleDesc.Quality = 0;
	swap_chain_description.Windowed = TRUE;
	swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
	D3D_FEATURE_LEVEL feature_level;

	HRESULT result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, feature_levels, 2, D3D11_SDK_VERSION, &swap_chain_description, &Globals::Overlay::dxgi_swap_chain, &Globals::Overlay::d3d11_device, &feature_level, &Globals::Overlay::d3d11_device_context);

	if (result == DXGI_ERROR_UNSUPPORTED) {
		result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0, feature_levels, 2, D3D11_SDK_VERSION, &swap_chain_description, &Globals::Overlay::dxgi_swap_chain, &Globals::Overlay::d3d11_device, &feature_level, &Globals::Overlay::d3d11_device_context);
	}

	if (result != S_OK) {
		if (Globals::Overlay::d3d11_render_target_view) Globals::Overlay::d3d11_render_target_view->Release();
		if (Globals::Overlay::dxgi_swap_chain) Globals::Overlay::dxgi_swap_chain->Release();
		if (Globals::Overlay::d3d11_device_context) Globals::Overlay::d3d11_device_context->Release();
		if (Globals::Overlay::d3d11_device) Globals::Overlay::d3d11_device->Release();
		return false;
	}

	ID3D11Texture2D* back_buffer = nullptr;
	Globals::Overlay::dxgi_swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));

	if (back_buffer) {
		Globals::Overlay::d3d11_device->CreateRenderTargetView(back_buffer, nullptr, &Globals::Overlay::d3d11_render_target_view);
		back_buffer->Release();
	}

	SetWindowPos(Globals::Overlay::WindowHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(Globals::Overlay::WindowHandle);
	ImGui_ImplDX11_Init(Globals::Overlay::d3d11_device, Globals::Overlay::d3d11_device_context);

	Globals::Fonts::inter_bold = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(inter_bold, sizeof(inter_bold), 20.0f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	return true;
}

bool UI::Render()
{
	MSG msg = { NULL };
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessageA(&msg, Globals::Overlay::WindowHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		UI::Menu(); 

		ImGui::Render();
		const float clear_color_with_alpha[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		Globals::Overlay::d3d11_device_context->OMSetRenderTargets(1, &Globals::Overlay::d3d11_render_target_view, nullptr);
		Globals::Overlay::d3d11_device_context->ClearRenderTargetView(Globals::Overlay::d3d11_render_target_view, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		Globals::Overlay::dxgi_swap_chain->Present(Globals::Overlay::VSync ? 1 : 0, 0);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	DestroyWindow(Globals::Overlay::WindowHandle);

	return true;
}

void UI::Styles()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.FrameRounding = 5;
	style.WindowRounding = 3;
	style.ItemSpacing = ImVec2(8, 8);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.IndentSpacing = 21;
	style.ScrollbarSize = 7;
	style.GrabMinSize = 5;
	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 1;
	style.ChildRounding = 0;
	style.FrameRounding = 0;
	style.PopupRounding = 3;
	style.ScrollbarRounding = 0;
	style.GrabRounding = 0;
	style.SelectableTextAlign = ImVec2(0.5f, 0.5f);

	style.Colors[ImGuiCol_ChildBg] = ImColor(30, 30, 30, 30);
}

void UI::Menu()
{
	static bool Menu = true;
	if (GetAsyncKeyState(VK_INSERT) & 1)
		Menu = !Menu;

	ESP();

	if (Menu)
	{
		SetWindowLong(Globals::Overlay::WindowHandle, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW);

		ImGui::SetNextWindowSize({ 580, 700 });
		ImGui::Begin("Roblox External Base | Made by @rtlcopymemory", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
		{
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 windowSize = ImGui::GetWindowSize();
			ImDrawList* draw = ImGui::GetWindowDrawList();

			draw->AddRectFilled(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y), IM_COL32(30, 30, 30, 255), 15.0f, ImDrawFlags_RoundCornersAll);

			draw->AddRectFilled(ImVec2(windowPos.x + 10.0f, windowPos.y + 10.0f), ImVec2(windowPos.x + windowSize.x - 10.0f, windowPos.y + 60.0f), ImColor(40, 40, 40, 255), 12.0f, ImDrawFlags_RoundCornersAll);
			ImGui::SetCursorPos(ImVec2(10, 15));

			static int Tab = 0;
			ImGui::PushFont(Globals::Fonts::inter_bold);

			if (ImGui::Tabs(0 == Tab, "Aiming", ImVec2(180, 35), NULL)) Tab = 0;
			if (ImGui::Tabs(1 == Tab, "Visuals", ImVec2(180, 35), NULL)) Tab = 1;
			if (ImGui::Tabs(2 == Tab, "Settings", ImVec2(180, 35), NULL)) Tab = 2;

			ImGui::PopFont();

			ImGui::SetCursorPos(ImVec2(10, ImGui::GetCursorPosY() + 60));

			if (Tab == 1)
			{
				ImVec2 childSize = ImVec2((windowSize.x - 30) * 0.5f, 600);

				ImGui::Columns(2, "TwoColumns", true);

				ImGui::BeginChild("Main", childSize);
				{
					ImGui::Text(" Visuals");

					ImGui::Checkbox(" ESP idk lol", &Globals::Features::ESPEnabled);

					ImGui::EndChild();
				}

				ImGui::NextColumn();
				ImGui::BeginChild("Other", childSize);
				{
					ImGui::Text(" Other");
					ImGui::EndChild();
				}

				ImGui::Columns(1);
			}
		}

		ImGui::End();
	}

	else
	{
		SetWindowLong(Globals::Overlay::WindowHandle, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
	}
}

