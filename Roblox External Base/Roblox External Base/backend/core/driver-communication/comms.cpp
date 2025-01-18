#include "comms.hpp"

// ik its not called this 
namespace DriverProperties {
    HANDLE Handle;
    INT32 TargetProcessID;
}

bool driver::Setup() {
    DriverProperties::Handle = CreateFile("\\\\.\\skar", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    return DriverProperties::Handle != INVALID_HANDLE_VALUE;
}

bool driver::IsValidAddress(uint64_t address) {
    return address > 0x400000 && address != 0xCCCCCCCCCCCCCCCC && address <= 0x7FFFFFFFFFFFFFFF;
}

void driver::ReadPhysical(PVOID address, PVOID buffer, DWORD size) {
    if (!IsValidAddress((uint64_t)address)) return;

    _rw arguments = { code_security, DriverProperties::TargetProcessID, (ULONGLONG)address, (ULONGLONG)buffer, size, FALSE };
    DeviceIoControl(DriverProperties::Handle, code_rw, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
}

void driver::WritePhysical(PVOID address, PVOID buffer, DWORD size) {
    if (!IsValidAddress((uint64_t)address)) return;

    _rw arguments = { code_security, DriverProperties::TargetProcessID, (ULONGLONG)address, (ULONGLONG)buffer, size, TRUE };
    DeviceIoControl(DriverProperties::Handle, code_rw, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
}

uintptr_t driver::FindImageAddress() {
    uintptr_t image_address = { NULL };
    _ba arguments = { code_security, DriverProperties::TargetProcessID, (ULONGLONG*)&image_address };
    DeviceIoControl(DriverProperties::Handle, code_ba, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
    return image_address;
}

uintptr_t driver::GetGaurdedRegionAddress() {
    uintptr_t guarded_region_address = { NULL };
    _ga arguments = { code_security, (ULONGLONG*)&guarded_region_address };
    DeviceIoControl(DriverProperties::Handle, code_get_guarded_region, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
    return guarded_region_address;
}

INT32 driver::GetProcessID(LPCTSTR process_name) {
    PROCESSENTRY32 pt;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hsnap, &pt)) {
        do {
            if (!lstrcmpi(pt.szExeFile, process_name)) {
                CloseHandle(hsnap);
                DriverProperties::TargetProcessID = pt.th32ProcessID;
                return pt.th32ProcessID;
            }
        } while (Process32Next(hsnap, &pt));
    }
    CloseHandle(hsnap);
    return NULL;
}

