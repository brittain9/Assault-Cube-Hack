#include "pch.h"
#include "hook.h"


bool Detour32(BYTE* toHook, BYTE* ourFunct, int len)
{
    if (len < 5) return false;
    // 0xE9 which is the jmp instruction to use to detour is 5 bytes. 1 byte for opcode, 4 bytes for 32bit relative offset.

    DWORD curProtection;
    VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);
    // make memory READWRITE then store old protection in address of curProtection.

    memset(toHook, 0x90, len);
    // Set memory in area that we are detouring to Nop. For some sort of safety measure.

    // instruction_pointer = instruction_pointer + 5 + relative_offset;
    uintptr_t relativeAddress = ((uintptr_t)ourFunct - (uintptr_t)toHook) - 5;
    // We subtract five because that’s the length of the jump instruction which the CPU adds when executing this instruction, and function_A is subtracted from function_B because it’s a relative jump

    // set jmp instruction in function
    *(BYTE*)toHook = 0xE9;
    // go 1 byte pasted jmp then insert relative address. Lots of weird casting.
    *(uintptr_t*)((uintptr_t)toHook + 1) = relativeAddress;

    VirtualProtect(toHook, len, curProtection, &curProtection);
    // returns region of memory to old protection.

    return true;
}

BYTE* TrampHook32(BYTE* toHook, BYTE* ourFunct, const intptr_t len)
{
    // http://jbremer.org/x86-api-hooking-demystified/#ah-basic for more info.

    // Make sure the length is greater than 5
    if (len < 5) return 0;

    // Create the gateway (len + 5 for the overwritten bytes + the jmp)
    void* gateway = VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    //Write the stolen bytes into the gateway
    memcpy(gateway, toHook, len);

    // Get the gateway to destination addy
    intptr_t  gatewayRelativeAddr = ((intptr_t)toHook - (intptr_t)gateway) - 5;

    // Add the jmp opcode to the end of the gateway
    *(char*)((intptr_t)gateway + len) = 0xE9;

    // Add the address to the jmp
    *(intptr_t*)((intptr_t)gateway + len + 1) = gatewayRelativeAddr;

    // Perform the detour
    Detour32(toHook, ourFunct, len);

    return (BYTE*)gateway;
}

void Hooker(const char* function, uintptr_t& oFunction, void* hFunction)
{
    HMODULE hMod = GetModuleHandle(L"opengl32.dll");

    if (hMod)
    {
        oFunction = (uintptr_t)TrampHook32((BYTE*)GetProcAddress(hMod, function), (BYTE*)hFunction, 5);
    }
}

Hook::Hook(BYTE* src, BYTE* dst, BYTE* PtrToGateWayFnPtr, uintptr_t len)
{
    // Basic Constructor
    this->src = src;
    this->dst = dst;
    this->len = len;
    this->PtrToGatewayFnPtr = PtrToGateWayFnPtr;
}

Hook::Hook(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len)
{
    // Wrapper around original constructor
    HMODULE hMod = GetModuleHandleA(modName);

    this->src = (BYTE*)GetProcAddress(hMod, exportName);
    this->dst = dst;
    this->len = len;
    this->PtrToGatewayFnPtr = PtrToGatewayFnPtr;
}

void Hook::Enable()
{
    memcpy(originalBytes, src, len);
    // Copy bytes so we can restore them later
    *(uintptr_t*)PtrToGatewayFnPtr = (uintptr_t)TrampHook32(src, dst, len);
    // Perform the hook
    bStatus = true;
}

void Hook::Disable()
{
    mem::Patch(src, originalBytes, len);
    bStatus = false;
}

void Hook::Toggle()
{
    if (!bStatus) Enable();
    else Disable();
}