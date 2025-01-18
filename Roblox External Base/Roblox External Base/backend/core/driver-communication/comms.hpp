#ifndef COMMS_HPP
#define COMMS_HPP

#include <cstddef>
#include <cstdint>
#include <basetsd.h>
#include <WTypesbase.h>
#include <TlHelp32.h>

// this shit was written by a drunk person, btw this is skar driver
#define code_rw CTL_CODE(FILE_DEVICE_UNKNOWN, 0x71, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_ba CTL_CODE(FILE_DEVICE_UNKNOWN, 0x72, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_get_guarded_region CTL_CODE(FILE_DEVICE_UNKNOWN, 0x73, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_security 0x85b3e12

typedef struct _rw {
    INT32 security;
    INT32 process_id;
    ULONGLONG address;
    ULONGLONG buffer;
    ULONGLONG size;
    BOOLEAN write;
} rw, * prw;

typedef struct _ba {
    INT32 security;
    INT32 process_id;
    ULONGLONG* address;
} ba, * pba;

typedef struct _ga {
    INT32 security;
    ULONGLONG* address;
} ga, * pga;

namespace DriverProperties {
    extern HANDLE Handle;
    extern INT32 TargetProcessID;
}

class driver {
public:
    INT32 GetProcessID(LPCTSTR process_name);

    bool Setup();
    bool IsValidAddress(uint64_t address);
    void ReadPhysical(PVOID address, PVOID buffer, DWORD size);
    void WritePhysical(PVOID address, PVOID buffer, DWORD size);

    template <typename T>
    T Rpm(uint64_t address) {
        T buffer{};
        driver::ReadPhysical((PVOID)address, &buffer, sizeof(T));
        return buffer;
    }

    template <typename T>
    T Wpm(uint64_t address, T buffer) {
        driver::WritePhysical((PVOID)address, &buffer, sizeof(T));
        return buffer;
    }

    uintptr_t FindImageAddress();
    uintptr_t GetGaurdedRegionAddress();
}; static driver* pMem = new driver;

#endif // COMMS_HPP