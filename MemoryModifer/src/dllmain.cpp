#include "pch.h"
#include "mem.h"
#include "hook.h"
#include "gldraw.h"
#include "gltext.h"
#include "assaultcube.h"
#include "geom.h"
#include "esp.h"

// You inject your DLL which contains a function with the same declaration as the function you're hooking. It has the same arguments, return type and calling convention
typedef BOOL (__stdcall * wglSwapBuffers_t) (_In_ HDC hDC);

wglSwapBuffers_t owglSwapBuffers;
wglSwapBuffers_t wglSwapBuffersGateway;
BOOL WINAPI hkwglSwapBuffers(HDC hDc);

Hook SwapBuffersHook("wglSwapBuffers", "opengl32.dll", (BYTE*)hkwglSwapBuffers, (BYTE*)&wglSwapBuffersGateway, 5);

// If HMODULE GetModuleHandleW(LPCSTR lpModuleName) parameter is NULL, returns a handle to the file used to create the calling process. In this case .DLL is inject into .exe, therefore function returns handle to the module. Handle to a module is the base address of module in memory.
uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);
HMODULE dllModule = nullptr;

bool bHealth = false, bAmmo = false, bRecoil = false, bESP = false;

GL::Font glFont;
const int FONT_HEIGHT = 15;
const int FONT_WIDTH = 9;
// Using Consolas mono spaced. Font width is always 9.

esp ESP;

int i = 1; // delete this

void Draw()
{
	HDC currentHDC = wglGetCurrentDC();
	// get currentHDC every frame

	if (!glFont.bBuilt || currentHDC != glFont.hdc)
	{
		// initialize first build
		glFont.Build(FONT_HEIGHT);
	}

	GL::SetupOrtho();

	ESP.Draw(glFont);

	GL::RestoreGL();
}

void HackLoop()
{

	if(i == 1)
	{
		std::cout << glGetString(GL_VERSION);
		i++;
	}
	if (GetAsyncKeyState(VK_F1) & 1)
		bHealth = !bHealth;

	if (GetAsyncKeyState(VK_F2) & 1)
		bAmmo = !bAmmo;

	if (GetAsyncKeyState(VK_F4) & 1)
		bESP = !bESP;

	//no recoil NOP
	if (GetAsyncKeyState(VK_F3) & 1)
	{
		bRecoil = !bRecoil;

		if (bRecoil)
		{
			mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
		}

		else
		{
			mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
		}
	}


	if (ESP.localPlayer)
	{
		if (bHealth)
			ESP.localPlayer->health = 1500;

		if (bAmmo)
			ESP.localPlayer->weaponPtr->weaponClipPtr->clip = 1500;
	}
}

BOOL WINAPI hkwglSwapBuffers(HDC hDc)
{
	HackLoop();
	// Always draw after coding hacking stuff.
	if( bESP)
	{
		Draw();
	}

	if (GetAsyncKeyState(VK_F12) & 1)
	{
		SwapBuffersHook.Disable();
		FreeLibrary(dllModule);
	}
	
	return wglSwapBuffersGateway(hDc);
}

DWORD WINAPI dwMainThread(HMODULE hModule)
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	SwapBuffersHook.Enable();

	ExitThread(0);
}

BOOL __stdcall DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		dllModule = hModule;
		DisableThreadLibraryCalls(hModule);
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)dwMainThread, 0, 0, nullptr));
		// Creates a thread which runs immediately after execution (because dwCreationFlags is 0), the address then starts at dwMainThread
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		FreeConsole();
		break;
	}
	return TRUE;
}