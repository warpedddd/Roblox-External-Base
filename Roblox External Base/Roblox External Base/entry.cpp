#include <Windows.h>
#include "frontend/core/user-interface/overlay.hpp"
#include <iostream>
#include "utilities/console.hpp"
#include "backend/core/driver-communication/comms.hpp"
#include "frontend/core/user-interface/user-interface.hpp"
#include <TlHelp32.h>
#include "utilities/globals/globals.hpp"
#include "backend/utilities/logs.hpp"
#include "backend/dependencies/offsets.hpp"

INT32 GetProcessID(LPCTSTR ProcName)
{
    PROCESSENTRY32 pt;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &pt)) {
        do {
            if (!lstrcmpi(pt.szExeFile, ProcName)) {
                CloseHandle(hSnapshot);
                return pt.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &pt));
    }

    CloseHandle(hSnapshot);
    return NULL;
}

int main()
{
	SetConsoleTitle("Roblox External Base | Made by @rtlcopymemory");

	while (true)
	{
		DriverProperties::TargetProcessID = GetProcessID("RobloxPlayerBeta.exe");
		if (DriverProperties::TargetProcessID)
		{
			console->LogDebug("Process ID", DriverProperties::TargetProcessID);
			break;
		}
	}

	if (!pMem->Setup()) console->Log(LogLevel::Error, "Failed to setup driver"); // Make sure you have mapped driver.sys 

    console->LogDebug("Base Address", pMem->FindImageAddress()); 

    while (true) {
        Globals::Roblox::VisualEngine = (pMem->Rpm<Visualengine>(logs->GetRenderView() + 0x10));
        Globals::Roblox::Datamodel = static_cast<GameInstance>((pMem->Rpm<std::uint64_t>(logs->GetRenderView() + 0x118) + DATAMODEL));

        if (Globals::Roblox::VisualEngine.Address && Globals::Roblox::Datamodel.Address) {
            console->LogDebug("VisualEngine", Globals::Roblox::VisualEngine.Address);
            console->LogDebug("Datamodel", Globals::Roblox::Datamodel.Address);
            break;
        }
    }

    Globals::Roblox::Players = Globals::Roblox::Datamodel.FindFirstChildOfClass("Players"); // we do it by the class name because then we don't need to do it game-specific ifykwim
    Globals::Roblox::Workspace = Globals::Roblox::Datamodel.FindFirstChildOfClass("Workspace"); // we do it by the class name because then we don't need to do it game-specific ifykwim

    Globals::Roblox::LocalPlayer = Globals::Roblox::Players.GetLocalPlayer();

    console->LogDebug("Workspace", Globals::Roblox::Workspace.Address);
    console->LogDebug("Players", Globals::Roblox::Players.Address);
    console->LogDebug("Local Player", Globals::Roblox::LocalPlayer.Address);

	if (!overlay->InitDX11Overlay()) console->Log(LogLevel::Error, "Failed to initiate DX11 Overlay"); // Direct X is my opp but I gave him a chance this time :D (magnifier overlay on top)
	if (!ui->Setup()) console->Log(LogLevel::Error, "Failed to setup ImGui");
	if (!ui->Render()) console->Log(LogLevel::Error, "Failed to start Menu");

	console->Log(LogLevel::Info, "Success");
}