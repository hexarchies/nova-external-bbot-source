#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#define FortPTR uintptr_t

inline const char* convertWideToNarrow(const wchar_t* wideString) {
    if (wideString == nullptr) {
        return nullptr;
    }

    size_t bufferSize = wcstombs(nullptr, wideString, 0);
    if (bufferSize == static_cast<size_t>(-1)) {
        return nullptr;
    }

    char* narrowString = new char[bufferSize + 1]; // +1 for null terminator

    if (wcstombs(narrowString, wideString, bufferSize + 1) == static_cast<size_t>(-1)) {
        delete[] narrowString;
        return nullptr;
    }

    narrowString[bufferSize] = '\0';

    return narrowString;
}

class memory
{
public:
    inline static DWORD GameProcessId{};
    inline static HANDLE GameHandle{};
    inline static uintptr_t GameBase{};

    static uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* moduleName);
    static DWORD GetProcessId(const wchar_t* processName);

    template <typename T>
    static T ReadMemory(uintptr_t address, SIZE_T size = sizeof(T));

    template <typename T>
    static void WriteMemory(uintptr_t address, T buffer, SIZE_T size = sizeof(T));
};

template<typename T>
inline T memory::ReadMemory(uintptr_t address, SIZE_T size)
{
    T buffer{};
    size_t bytes{};
    ReadProcessMemory(GameHandle, (PVOID)address, &buffer, size, &bytes);
    return buffer;
}

template<typename T>
inline void memory::WriteMemory(uintptr_t address, T buffer, SIZE_T size)
{
    size_t bytes{};
    WriteProcessMemory(GameHandle, (PVOID)address, &buffer, size, &bytes);
}