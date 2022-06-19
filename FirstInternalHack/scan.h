#pragma once
#include <Windows.h>
#include <string>
#include "ntdll.h"

char* ScanBasic(char* pattern, char* mask, char* begin, intptr_t size);
char* ScanInternal(char* pattern, char* mask, char* begin, intptr_t size);
