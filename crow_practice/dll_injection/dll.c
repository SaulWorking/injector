//compile using 
// "clang++ -m64 -o shellcode.exe shellcode.c"

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

#define DECOR() printf("==================================================================\n")
#define INFO(MSG, ...) printf("[-_-] "  MSG  "\n", ##__VA_ARGS__)
#define GOOD(MSG, ...) printf("[^_^]"  MSG  "\n", ##__VA_ARGS__)
#define BAD(MSG, ...) printf("[>:(]"  MSG  "\n", ##__VA_ARGS__)
#define ERR(MSG, ...) {	DECOR();				          \
			fprintf(stderr,				      	          \
	      	      		"[EEK] "  MSG "\n"		       	  \
				"[EEK] ERROR CODE:      0x%ld\n"          \
				"[EEK] FUNCTION:        %s()\n" 	      \
				"[EEK] LINE: 	       %ld\n",            \
				GetLastError(),__func__,__LINE__);     	  \
				DECOR();			          			  \
		      }

DWORD getPID(_In_ const char* processName);
HANDLE inject(_In_ DWORD PID);
VOID CleanHandleWait(
		_In_ HANDLE hProcess,
		_In_ HANDLE hThread
		);


const char* dllPath[MAX_PATH] = "C:\\Users\\thien\\Desktop\\injector\\crow_practice\\dll\\Dll1\\x64\\Debug\\Dll1.dll";		
const size_t dllPathSize = sizeof(dllPath);

int main(void)
{
	DWORD PID = getPID("mspaint.exe");
	if(PID == -1)
	{
		return EXIT_FAILURE;
	}

	HMODULE hKernel32;

	INFO("PID: %lu",PID);
	inject(PID);
	INFO("END");

	return EXIT_SUCCESS;
}

DWORD getPID(_In_ const char* processName)
{
	HANDLE hProcessSnap = NULL;	//list of processes
	PROCESSENTRY32 pe32;		//info of process
	
	hProcessSnap = CreateToolhelp32Snapshot(
			TH32CS_SNAPPROCESS, //snapshot Process flag
			0		   		    //Process ID
			);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		ERR("SNAPSHOT NOT FOUND");
		return -1;
	}

	GOOD("Took a snapshot...");

	pe32.dwSize = sizeof(PROCESSENTRY32);	//required for Process32 function

	if(Process32First(
		hProcessSnap, 			//list of processes
		&pe32	      			//change pe32 value
		))
	{
		do{
			if(strcmp(pe32.szExeFile, processName) == 0)
			{
				GOOD("FOUND PID FOR: %s", processName);
				CloseHandle(hProcessSnap);
				return pe32.th32ProcessID;
			}	
		}while(Process32Next(hProcessSnap, &pe32));
	}

	ERR("No PID found...");
	CloseHandle(hProcessSnap);
	return -1;
}

HANDLE inject(_In_ DWORD PID)
{
	HANDLE hProcess = OpenProcess(
		PROCESS_ALL_ACCESS, 					//access flags
		FALSE, 									//handle inheritance
		PID										//DWORD Process ID
	);
	
	if(hProcess == INVALID_HANDLE_VALUE)
	{
		ERR("COULD NOT OPEN PROCESS");
		return NULL;
	}

	GOOD("PROCESS HAS BEEN OPENED");

	LPVOID lpBaseAddress = VirtualAllocEx(
		hProcess,								//process to allocate to
		NULL,									//starting address
		dllPathSize,							//size of memory region
		(MEM_COMMIT | MEM_RESERVE),				//allocation type flags
		PAGE_READWRITE							//memory protection flags (suspicious)
	);


	if(lpBaseAddress == NULL)
	{
		ERR("COULD NOT ALLOCATE MEMORY");
		return NULL;
	}

	GOOD("MEMORY HAS BEEN ALLOCATED");

	BOOL isWritten = WriteProcessMemory(
		hProcess,							     //process to allocate to
		lpBaseAddress,						     //starting address
		dllPath,						    	 //LPCVOID buffer (pointer  to a constant of any type)
		dllPathSize,		     				//# of bytes to be written (in)
		NULL							         //# of bytes written	   (out)
	);
	
	if(!isWritten)
	{
		BAD("COULD NOT WRITE TO PROCESS MEMORY");
		return NULL;
	}

	GOOD("MEMORY HAS BEEN WRITTEN");	
/* 
	HANDLE	hThread = CreateRemoteThreadEx(
		hProcess,			 				   	 //process to create thread
		NULL,    								 //security attributes
		0,	 		       						 //dwStackSize (See Thread Stack Size on the MSDN for more information.)
		(LPTHREAD_START_ROUTINE)lpBaseAddress,	 //starting address from memory allocated
		NULL,									 //pointer to be passed through to the thread function (no need?)
		0,    									 //creation behavior flag
		NULL,									 //lpAttributeList (in)
		NULL									 //lpThreadId (out)
	);

	if(hThread == INVALID_HANDLE_VALUE)
	{
		ERR("COULD NOT CREATE THREAD");
		return NULL;
	}

	GOOD("THREAD CREATED");
*/
	CleanHandleWait(hProcess,hProcess);
}



VOID CleanHandleWait(HANDLE hProcess, HANDLE hThread){
	switch(WaitForSingleObject(hProcess,INFINITE)){
		case WAIT_OBJECT_0:
		INFO("Process is closing...");
			if(hProcess != NULL){CloseHandle(hProcess);}
			if(hThread != NULL){CloseHandle(hThread);}
		GOOD("HANDLES CLOSED");
		break;

		case WAIT_TIMEOUT:
		break;

		case WAIT_FAILED:
			if(hProcess != NULL){CloseHandle(hProcess);}
			if(hThread != NULL){CloseHandle(hThread);}
		ERR("PROCESS WAIT FAILED");
		break;

		default:
		ERR("UNKNOWN SIGNAL");
	}
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
