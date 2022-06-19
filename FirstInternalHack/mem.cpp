#include "mem.h"

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	// Read Nop function for more info. This function instead uses memcpy to change the memory to whatever the source is
	// in this case, the src is the original opcodes of function we Nop.
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
	// External
	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
}

void mem::Nop(BYTE* dst, unsigned int size)
{
	/* VirtualProtect changes the protection on a region of memory in the virtual address space.
	 * First oldprotect, a temp variable to store the old protection status, is set up.
	 * VirtualProtect then receives the address of the starting page (page = smallest unit of data for
	 * memory management in a virtual memory operating system), then the size of the region whose protection
	 * will be changed. Then it takes the new protection, in this case READWRITE, then the address of the
	 * temp variable to store the current protection that will be replaced by READWRITE.
	 * Once we change the protection, this function sets the memory to NOP (0x90), then returns old protection.
	 */
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess)
{
	// External
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);

	PatchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;
}

uintptr_t mem::FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	// This function takes an ptr and adds offsets to it.
	uintptr_t addr = ptr;
	for (int i{ 0 }; i < offsets.size(); i++)
	{
		addr = *(uintptr_t*)addr;
		// This casts addr to be a uintptr_t ptr then dereferences and stores the value back in addr.
		// This does something with the way offsets are added.
		addr += offsets[i];
	}
	return addr;
}