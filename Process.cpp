/*

	The process functions are used to gather information about processes, modules, etc.
	Code by Z0F.

*/

#include <iostream>
#include "Process.h"

// Get Process ID From an executable name using toolhelp32Snapshot:
DWORD GetProcID(const wchar_t* procName) {
	PROCESSENTRY32 procEntry = { 0 };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!hSnapshot) { return 0; }

	procEntry.dwSize = sizeof(procEntry);

	if (!Process32First(hSnapshot, &procEntry)) { return 0; }

	do {
		if (!wcscmp(procEntry.szExeFile, procName)) {
			CloseHandle(hSnapshot);
			return procEntry.th32ProcessID;
		}
	} while (Process32Next(hSnapshot, &procEntry));

	CloseHandle(hSnapshot);
	return 0;
}

// Get ModuleEntry from module name, using toolhelp32snapshot:
MODULEENTRY32 GetModule(const DWORD &procID, wchar_t* modName) {
	MODULEENTRY32 modEntry = { 0 };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);

	if (hSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 curr = { 0 };

		curr.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &curr)) {
			do {
				if (!wcscmp(curr.szModule, modName)) {
					modEntry = curr;
					std::wcout << "Module Entry for \"" << modName << "\" found.\n";
					break;
				}
			} while (Module32Next(hSnapshot, &curr));
		}
		CloseHandle(hSnapshot);
	}
	return modEntry;
}