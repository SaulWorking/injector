#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>



#define DECOR() printf("==================================================================\n")
#define INFO(MSG, ...) printf("[HI] "  MSG  "\n", ##__VA_ARGS__)
#define GOOD(MSG, ...) printf("[W]"  MSG  "\n", ##__VA_ARGS__)
#define BAD(MSG, ...) printf("[L]"  MSG  "\n", ##__VA_ARGS__)

DWORD getPID(_In_ const char* processName);

int main(void)
{

	DWORD PID = getPID("notepad.exe");

	INFO("PID: %lu",PID);
	return EXIT_SUCCESS;
}

DWORD getPID(_In_ const char* processName)
{
	INFO("Finding PID...");

	HANDLE hProcessSnap;	//list of processes
	PROCESSENTRY32  pe32; 	//info of process
	
	hProcessSnap = CreateToolhelp32Snapshot(
			TH32CS_SNAPPROCESS, //TOOL FLAG
			0		    //Process ID
			);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		BAD("ERROR");
		return -1;
	}

	INFO("Took a snapshot...");

	pe32.dwSize = sizeof(PROCESSENTRY32);	//required for Process32 function

	if(Process32First(
		hProcessSnap, //list of processes
		&pe32	//change pe32 value
		))
	{

		do{
			if(strcmp(pe32.szExeFile, processName) == 0)
			{
				CloseHandle(hProcessSnap);
				return pe32.th32ProcessID;
			}	
		}while(Process32Next(hProcessSnap, &pe32));
	}

	BAD("No PID found...");
	CloseHandle(hProcessSnap);
	return -1;
}

HANDLE work(_In_ DWORD PID)
{
	HANDLE hProcess = OpenProcess(
						PROCESS_VM_READ | PROCESS_VM_WRITE,
						FALSE, 
						PID
	);
	LPVOID lpAddress = VirtualAllocEx(
		hProcess,
		NULL,
		100,
		NULL,
		NULL
	);



}

/**
 * 
 * 
 *  Get a handle on a process by attaching to or creating one. 

	Allocate some memory in the process with the necessary/appropriate permissions for our shellcode to run.

	Write the contents of your shellcode/payload to that allocated page in the process memory.

	Create a thread that'll start running what you've surgically allocated and written into the process!

	Optional: Incorporate some cleanup and housekeeping once the thread's finished its execution.
 */
/** 
HANDLE OpenProcess(
  [in] DWORD dwDesiredAccess,
  [in] BOOL  bInheritHandle,
  [in] DWORD dwProcessId
);
LPVOID VirtualAllocEx(
  [in]           HANDLE hProcess,
  [in, optional] LPVOID lpAddress,
  [in]           SIZE_T dwSize,
  [in]           DWORD  flAllocationType,
  [in]           DWORD  flProtect
);

BOOL WriteProcessMemory(
  [in]  HANDLE  hProcess,
  [in]  LPVOID  lpBaseAddress,
  [in]  LPCVOID lpBuffer,
  [in]  SIZE_T  nSize,
  [out] SIZE_T  *lpNumberOfBytesWritten
);

HANDLE CreateRemoteThreadEx(
  [in]            HANDLE                       hProcess,
  [in, optional]  LPSECURITY_ATTRIBUTES        lpThreadAttributes,
  [in]            SIZE_T                       dwStackSize,
  [in]            LPTHREAD_START_ROUTINE       lpStartAddress,
  [in, optional]  LPVOID                       lpParameter,
  [in]            DWORD                        dwCreationFlags,
  [in, optional]  LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList,
  [out, optional] LPDWORD                      lpThreadId
);
*/