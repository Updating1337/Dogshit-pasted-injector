#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include "../ImLoader/protection/xorstr.h"
#include "../ImLoader/protection/antiDbg.h"
#include <Windows.h>
#include <iostream>
#include <thread>
#include <TlHelp32.h>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <assert.h>
#include <iostream>
#include <windows.h>
#include <urlmon.h>
#include <stdio.h>
#include <windows.h>
#include <winternl.h>
#include <vector>

typedef NTSTATUS(WINAPI* _NtUnmapViewOfSection)(HANDLE ProcessHandle, PVOID BaseAddress);

typedef struct BASE_RELOCATION_BLOCK {
	DWORD PageAddress;
	DWORD BlockSize;
} BASE_RELOCATION_BLOCK, * PBASE_RELOCATION_BLOCK;

typedef struct BASE_RELOCATION_ENTRY {
	USHORT Offset : 12;
	USHORT Type : 4;
} BASE_RELOCATION_ENTRY, * PBASE_RELOCATION_ENTRY;

bool map_file(const std::string& processName, const std::vector<uint8_t>& bytes) {
	

	LPSTARTUPINFOA pStartupinfo = new STARTUPINFOA();
	PROCESS_INFORMATION proc_info;
	HANDLE HEvilFile;
	if (!CreateProcessA(NULL, (LPSTR)processName.c_str(), NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, pStartupinfo, &proc_info)) { return false; }
	DWORD EvilFileSize = bytes.size();
	PBYTE EvilImage = new BYTE[EvilFileSize];
	memcpy(EvilImage, bytes.data(), EvilFileSize);
	CONTEXT pContext;
	pContext.ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(proc_info.hThread, &pContext)) { return false; }
	PVOID BaseAddress;

#ifdef _X86_
	ReadProcessMemory(proc_info.hProcess, (PVOID)(pContext.Ebx + 8), &BaseAddress, sizeof(PVOID), NULL);
#endif

#ifdef _WIN64
	ReadProcessMemory(proc_info.hProcess, (PVOID)(pContext.Rdx + (sizeof(SIZE_T) * 2)), &BaseAddress, sizeof(PVOID), NULL);
#endif
	HMODULE hNTDLL = GetModuleHandleA(skCrypt("ntdll"));
	FARPROC fpNtUnmapViewOfSection = GetProcAddress(hNTDLL, skCrypt("NtUnmapViewOfSection"));
	_NtUnmapViewOfSection NtUnmapViewOfSection = (_NtUnmapViewOfSection)fpNtUnmapViewOfSection;
	if (NtUnmapViewOfSection(proc_info.hProcess, BaseAddress)) { return false; }

	// Get the DOS Header and the NT Header of the mapped file
	PIMAGE_DOS_HEADER dos_head = (PIMAGE_DOS_HEADER)EvilImage;
	PIMAGE_NT_HEADERS nt_head = (PIMAGE_NT_HEADERS)((LPBYTE)EvilImage + dos_head->e_lfanew);

	// Allocate memory in the suspended process
	PVOID mem = VirtualAllocEx(proc_info.hProcess, BaseAddress, nt_head->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#ifdef _X86_
	// Calculate the offset of the suspended process base address from the file's base address
	DWORD BaseOffset = (DWORD)BaseAddress - nt_head->OptionalHeader.ImageBase;

	// Change the file's base address to the base address of the suspended process
	nt_head->OptionalHeader.ImageBase = (DWORD)BaseAddress;
#endif

#ifdef _WIN64
	// Calculate the offset of the suspended process base address from the file's base address
	DWORD64 BaseOffset = (DWORD64)BaseAddress - nt_head->OptionalHeader.ImageBase;

	// Change the file's base address to the base address of the suspended process
	nt_head->OptionalHeader.ImageBase = (DWORD64)BaseAddress;
#endif

	// Write the file's headers to the allocated memory in the suspended process
	if (!WriteProcessMemory(proc_info.hProcess, BaseAddress, EvilImage, nt_head->OptionalHeader.SizeOfHeaders, 0)) { return false; }

	// Write all the sections from the mapped file to the suspended process
	PIMAGE_SECTION_HEADER sec_head;
	// Loop over every section
	for (int i = 0; i < nt_head->FileHeader.NumberOfSections; i++) {
		// Get the head of the current section
		sec_head = (PIMAGE_SECTION_HEADER)((LPBYTE)EvilImage + dos_head->e_lfanew + sizeof(IMAGE_NT_HEADERS) + (i * sizeof(IMAGE_SECTION_HEADER)));
		// Write the section from the file into the allocated memory
		if (!WriteProcessMemory(proc_info.hProcess, (PVOID)((LPBYTE)mem + sec_head->VirtualAddress), (PVOID)((LPBYTE)EvilImage + sec_head->PointerToRawData), sec_head->SizeOfRawData, NULL)) { return false; }
	}

	// Check if there is an offset between the base addresses
	if (BaseOffset) {
		// Loop over every section
		for (int i = 0; i < nt_head->FileHeader.NumberOfSections; i++) {
			// Get the head of the current section
			sec_head = (PIMAGE_SECTION_HEADER)((LPBYTE)EvilImage + dos_head->e_lfanew + sizeof(IMAGE_NT_HEADERS) + (i * sizeof(IMAGE_SECTION_HEADER)));

			// Compare the section name to the ".reloc" section
			char pSectionName[] = ".reloc";
			if (memcmp(sec_head->Name, pSectionName, strlen(pSectionName))) {
				// If the section is not the ".reloc" section, continue to the next section
				continue;
			}

			// Get the address of the section data
			DWORD RelocAddress = sec_head->PointerToRawData;
			IMAGE_DATA_DIRECTORY RelocData = nt_head->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
			DWORD Offset = 0;

			// Iterate over the relocation table
			while (Offset < RelocData.Size) {
				// Get the head of the relocation block
				PBASE_RELOCATION_BLOCK pBlockHeader = (PBASE_RELOCATION_BLOCK)&EvilImage[RelocAddress + Offset];
				Offset += sizeof(BASE_RELOCATION_BLOCK);

				// Calculate the entries in the current table
				DWORD EntryCount = (pBlockHeader->BlockSize - sizeof(BASE_RELOCATION_BLOCK)) / sizeof(BASE_RELOCATION_ENTRY);
				PBASE_RELOCATION_ENTRY pBlocks = (PBASE_RELOCATION_ENTRY)&EvilImage[RelocAddress + Offset];

				for (int x = 0; x < EntryCount; x++) {
					Offset += sizeof(BASE_RELOCATION_ENTRY);

					// If the type of the entry is 0, we don't need to do anything
					if (pBlocks[x].Type == 0) { continue; }

					// Resolve the address of the relocation
					DWORD FieldAddress = pBlockHeader->PageAddress + pBlocks[x].Offset;

#ifdef _X86_
					// Read the value in that address
					DWORD EntryAddress = 0;
					ReadProcessMemory(proc_info.hProcess, (PVOID)((DWORD)BaseAddress + FieldAddress), &EntryAddress, sizeof(PVOID), 0);
					// Add the correct offset to that address and write it
					EntryAddress += BaseOffset;
					if (!WriteProcessMemory(proc_info.hProcess, (PVOID)((DWORD)BaseAddress + FieldAddress), &EntryAddress, sizeof(PVOID), 0)) { return false; }
#endif

#ifdef _WIN64
					// Read the value in that address
					DWORD64 EntryAddress = 0;
					ReadProcessMemory(proc_info.hProcess, (PVOID)((DWORD64)BaseAddress + FieldAddress), &EntryAddress, sizeof(PVOID), 0);
					// Add the correct offset to that address and write it
					EntryAddress += BaseOffset;
					if (!WriteProcessMemory(proc_info.hProcess, (PVOID)((DWORD64)BaseAddress + FieldAddress), &EntryAddress, sizeof(PVOID), 0)) { return false; }
#endif
				}
			}
		}
	}

#ifdef _X86_
	// Write the new image base address
	WriteProcessMemory(proc_info.hProcess, (PVOID)(pContext.Ebx + 8), &nt_head->OptionalHeader.ImageBase, sizeof(PVOID), NULL);

	// Write the new entry point
	DWORD EntryPoint = (DWORD)((LPBYTE)mem + nt_head->OptionalHeader.AddressOfEntryPoint);
	pContext.Eax = EntryPoint;
#endif

#ifdef _WIN64
	// Write the new image base address
	WriteProcessMemory(proc_info.hProcess, (PVOID)(pContext.Rdx + (sizeof(SIZE_T) * 2)), &nt_head->OptionalHeader.ImageBase, sizeof(PVOID), NULL);

	// Write the new entry point
	DWORD64 EntryPoint = (DWORD64)((LPBYTE)mem + nt_head->OptionalHeader.AddressOfEntryPoint);
	pContext.Rcx = EntryPoint;
#endif
	if (!SetThreadContext(proc_info.hThread, &pContext)) { return false; }
	if (!ResumeThread(proc_info.hThread)) { return false; }
	CloseHandle(proc_info.hThread);
	CloseHandle(proc_info.hProcess);
	delete[] EvilImage;
	delete pStartupinfo;
	LI_FN(exit)(-1);
	return true;
}

#pragma endregion

inline  void open_binary_file(const std::string& file, std::vector<uint8_t>& data)
{
	std::ifstream fstr(file, std::ios::binary);
	fstr.unsetf(std::ios::skipws);
	fstr.seekg(0, std::ios::end);

	const auto file_size = fstr.tellg();

	fstr.seekg(NULL, std::ios::beg);
	data.reserve(static_cast<uint32_t>(file_size));
	data.insert(data.begin(), std::istream_iterator<uint8_t>(fstr), std::istream_iterator<uint8_t>());
}


bool IsProcessRunning(const wchar_t* processName)
{
	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_wcsicmp((wchar_t*)entry.szExeFile, processName))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}


auto RegistryInject = [](std::wstring DLLPath, const wchar_t* process)
	{
		MessageBoxA(NULL, xorstr_("Dont Restart Pc With Rust Open"), xorstr_("Please Open Rust Immediately"), NULL);
		std::cout << (xorstr_("Open Rust Please\n"));

		const wchar_t* keyPath = xorstr_(L"SYSTEM\\ControlSet001\\Services\\WinSock2\\Parameters");
		const wchar_t* valueName = xorstr_(L"AutodialDLL");

		HKEY hKey;
		LONG openResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_SET_VALUE, &hKey);
		LONG setValueResult = RegSetValueExW(hKey, valueName, 0, REG_SZ, reinterpret_cast<const BYTE*>(DLLPath.c_str()), (DLLPath.length() + 1) * sizeof(wchar_t));
		RegCloseKey(hKey);


		while (true) {
			HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (snapshot == INVALID_HANDLE_VALUE) {
				return false;
			}

			PROCESSENTRY32 processEntry;
			processEntry.dwSize = sizeof(PROCESSENTRY32);

			// Enumerate through the processes
			if (Process32First(snapshot, &processEntry)) {
				do {
					std::wstring exeFileName(processEntry.szExeFile, processEntry.szExeFile + strlen(processEntry.szExeFile));

					if (wcscmp(exeFileName.c_str(), process) == 0) {
						Sleep(30000);
						const wchar_t* keyPath = xorstr_(L"SYSTEM\\ControlSet001\\Services\\WinSock2\\Parameters");
						const wchar_t* valueName = xorstr_(L"AutodialDLL");
						const wchar_t* newDllPath = xorstr_(L"C:\\Windows\\System32\\rasadhlp.dll"); // Replace with the actual path
						HKEY hKey;
						LONG openResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_SET_VALUE, &hKey);
						LONG setValueResult = RegSetValueExW(hKey, valueName, 0, REG_SZ, (const BYTE*)newDllPath, (wcslen(newDllPath) + 1) * sizeof(wchar_t));
						RegCloseKey(hKey);
						std::cout << (xorstr_("Press F1 In Main Menu To Load\n"));
						MessageBox(NULL, xorstr_("Sucess, loader will close 20s"), xorstr_("Sucess, loader will close 20s"), MB_OK);
						Sleep(20000);
						DeleteFileW(DLLPath.c_str());
						exit(0);

					}
				} while (Process32Next(snapshot, &processEntry));
			}

			CloseHandle(snapshot);

			Sleep(1000); // Sleep for a second before checking again

		}
		return true;
	};

void loadDll(std::vector<std::uint8_t> Bytes, const wchar_t* process)
{
	remove(xorstr_("C:\\Windows\\System32\\xwinput_09.dll"));
	std::vector<std::uint8_t> buffer;
	buffer = Bytes;
	if (buffer.empty())
	{
		MessageBox(NULL, xorstr_("Server"), xorstr_("Injection 0x2A"), NULL);
	}

	std::ofstream file(xorstr_("C:\\Windows\\System32\\xwinput_09.dll"), std::ios_base::binary | std::ios_base::out);

	for (int i = 0; i < buffer.size() - 1; i++)
		file.write((char*)&buffer.at(i), sizeof(uint8_t));

	file.close();

	std::wstring path = xorstr_(L"C:\\Windows\\System32\\xwinput_09.dll");
	MessageBox(NULL, xorstr_("Please Do Not Turn Off Pc"), xorstr_("Please Do Not Turn Off Pc"), MB_OK);
	RegistryInject(path, process);

	const wchar_t* keyPath = xorstr_(L"SYSTEM\\ControlSet001\\Services\\WinSock2\\Parameters");
	const wchar_t* valueName = xorstr_(L"AutodialDLL");
	const wchar_t* newDllPath = xorstr_(L"C:\\Windows\\System32\\rasadhlp.dll"); // Replace with the actual path
	HKEY hKey;
	LONG openResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_SET_VALUE, &hKey);
	LONG setValueResult = RegSetValueExW(hKey, valueName, 0, REG_SZ, (const BYTE*)newDllPath, (wcslen(newDllPath) + 1) * sizeof(wchar_t));
	RegCloseKey(hKey);
	exit(9);

}

