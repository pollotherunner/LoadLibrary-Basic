#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;

DWORD findPidByName(const wchar_t* name)
{
	PROCESSENTRY32W pe;
	pe.dwSize = sizeof(pe);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	DWORD pid = 0;

	Process32FirstW(hSnapshot, &pe);
	do
	{
		
		if(!wcscmp(name, pe.szExeFile))
		{
			pid = pe.th32ProcessID;
			break;
		}
	} while (Process32NextW(hSnapshot, &pe));

	return(pid);
}

int main()
{

	DWORD pid = findPidByName(L"processo_alvo.exe");
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (hProcess == INVALID_HANDLE_VALUE)
	{
		cout << "failed to create hProcess HANDLE\n";
	}
	
	const char* dllPath = "your dll path";

	LPVOID allocDllPathIntoMemory = VirtualAllocEx(hProcess, NULL, strlen(dllPath), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if(allocDllPathIntoMemory == nullptr)
	{
		cout << "failed VirtualAllocEx";
	}

	BOOL writeInMemory = WriteProcessMemory(hProcess, allocDllPathIntoMemory, dllPath, strlen(dllPath), NULL);
	
		if(!writeInMemory)
		{
			cout << "failed writeprocmemory\n";
		}
	
	LPVOID loadLibraryAddress = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");

	if(loadLibraryAddress == NULL)
	{
		cout << "Failed to get LoadLibraryA from kernel.dll\n";
	}

	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)loadLibraryAddress, allocDllPathIntoMemory, NULL, NULL);

	if (hRemoteThread == INVALID_HANDLE_VALUE)
	{
		cout << "Failed to create remote thread\n";
	}

	cout << "injected!\n";

	system("pause");

	return 0;

}
