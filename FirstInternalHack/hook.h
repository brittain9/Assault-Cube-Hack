#pragma once
#include <Windows.h>
#include "mem.h"

bool Detour32(BYTE* src, BYTE* dst, int len);
BYTE* TrampHook32(BYTE* src, BYTE* dst, const intptr_t len);
void Hooker(const char* function, uintptr_t& oFunction, void* hFunction);


struct Hook
{
	bool bStatus{ false };

	BYTE* src{ nullptr };
	BYTE* dst{ nullptr };
	BYTE* PtrToGatewayFnPtr{ nullptr };
	int len{ 0 };

	BYTE originalBytes[10]{ 0 };

	Hook(BYTE* src, BYTE* dst, BYTE* PtrToGateWayPtr, uintptr_t len);
	Hook(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGateWayPtr, uintptr_t len);

	void Enable();
	void Disable();
	void Toggle();

};