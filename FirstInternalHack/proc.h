#pragma once
#include <vector>
#include <windows.h>
#include <TlHelp32.h>

/*These functions are mainly for external processes.*/

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);