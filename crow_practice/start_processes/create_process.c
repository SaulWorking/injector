#include <windows.h>
#include <stdio.h>

#define DECOR() printf("==================================================================\n")
#define INFO(MSG, ...) printf("[HI] "  MSG  "\n", ##__VA_ARGS__)
#define GOOD(MSG, ...) printf("[W]"  MSG  "\n", ##__VA_ARGS__)
#define BAD(MSG, ...) printf("[L]"  MSG  "\n", ##__VA_ARGS__)

const wchar_t* app_path = L"C:\\Windows\\System32\\notepad.exe";	//application path

VOID DisplayProcessInfo(
		_In_ DWORD dwThreadId,
		_In_ DWORD dwProcessId,
		_In_ HANDLE hThread,
		_In_ HANDLE hProcess
){
	DECOR();
	INFO("PID: %ld\n",dwProcessId);
	INFO("TID: %ld\n",dwThreadId);
	INFO("Process Handle: %p\n",hProcess);
	INFO("Thread Handle: %p\n",hThread);
	DECOR();
}

VOID finishHandle(
		_In_	HANDLE hThread,
		_In_	HANDLE hProcess
){
	if(hThread != NULL){CloseHandle(hThread);}
	if(hProcess != NULL){CloseHandle(hProcess);}
}	

int main(void){

struct _STARTUPINFOW si = { 0 };
struct _PROCESS_INFORMATION pi = { 0 };

INFO("Trying to start process: %S",app_path);
	if(!CreateProcessW(
		app_path,
		NULL,	//command line commands
		NULL,	//process attributes
		NULL,	//thread attributes
		FALSE, //Inherit Handles?
		BELOW_NORMAL_PRIORITY_CLASS, // priority class
		NULL, //pointer to environment
		NULL, //current directory Path
			&si,	//START UP INFO POINTER
		&pi	//PROCESS INFO POINTER
	)){
BAD("PROCESS COULDN'T BE CREATED %ld",GetLastError());
	}	

GOOD("PROCESS HAS BEEN CREATED");
	DisplayProcessInfo(
		pi.dwProcessId,
		pi.dwThreadId,
		pi.hProcess,
		pi.hThread
	);

switch(WaitForSingleObject(pi.hProcess,INFINITE)){
	case WAIT_OBJECT_0:
	INFO("Process is closing...");
	finishHandle(pi.hProcess,pi.hThread);
	GOOD("HANDLES CLOSED");
	break;

	case WAIT_TIMEOUT:
	break;

	case WAIT_FAILED:
	finishHandle(pi.hProcess,pi.hThread);
	BAD("PROCESS WAIT FAILED");
	break;

	default:
	printf("(bye) idk nothing happened");
}
	return EXIT_SUCCESS;
}
