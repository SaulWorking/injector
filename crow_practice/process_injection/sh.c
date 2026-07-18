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
			TH32CS_SNAPPROCESS, //snapshot Process flag
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
		&pe32	      //change pe32 value
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
	SIZE_T MEEEE_SIZE = sizeof(PROCESSENTRY32);

	HANDLE hProcess = OpenProcess(
		PROCESS_VM_READ | PROCESS_VM_WRITE,	//access flags
		FALSE, //inheritance
		PID	
	);
	
	if(hProcess == INVALID_HANDLE_VALUE)
	{
		BAD("COULD NOT OPEN PROCESS");
		return NULL;
	}

	LPVOID lpBaseAddress = VirtualAllocEx(
		hProcess,			//process to allocate to
		NULL,				//starting address
		MEEEE_SIZE,			//size of memory region
		MEM_COMMIT | MEM_RESERVE,	//allocation type flags
		PAGE_EXECUTE_READWRITE		//memory protection flags
	);

	if(lpBaseAddress == NULL)
	{
		BAD("COULD NOT ALLOCATE MEMORY");
		return NULL;
	}

	BOOL isWritten = WriteProcessMemory(
		hProcess,	//process to allocate to
		lpBaseAddress,	//starting address
		L"6767676767",	//LPCVOID buffer
		MEEEE_SIZE	//# of bytes to be written (in)
		NULL	//# of bytes written	(out)
	);
	
	if(!isWritten)
	{
		BAD("COULD NOT WRITE TO PROCESS MEMORY");
		return NULL;
	}

		
	HANDLE	hThread = CreateRemoteThreadEx(
		hProcess, //process to create thread
		NULL,     //security attributes
		MEEEE_SIZE, //dwStackSize (See Thread Stack Size on the MSDN for more information.)
		lpBaseAddress,	//starting address from memory allocated
		NULL, //pointer to be passed through to the thread function (no need?)
		0,     //creation behavior flag
		NULL,	//lpAttributeList (in)
		NULL	//lpThreadId (out)
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
