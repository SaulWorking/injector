#include <stdio.h>
#include <windows.h>

//[LPCWSTR] is a WCHAR const*
//[LPWSTR] is a WCHAR *

int main(void)
{

struct _STARTUPINFOW si = { 0 }; //*LPSTARTUPINFOW
struct _PROCESS_INFORMATION pi = { 0 };

	if(!CreateProcessW(
		L"C:\\Windows\\System32\\notepad.exe",	//application path [LPCWSTR]
		NULL,	//command line commands [LPWSTR] (not needed) (max length is 32767 char)
		NULL, //pointer for child processes  [LPSECURITY_ATTRIBUTES]
		NULL, //pointer for child processes inheriting handle [LPSECURITY_ATTRIBUTES]
		FALSE, //bInheritHandles
		NORMAL_PRIORITY_CLASS, // PRIORITY CLASS
		NULL, // environment block
		NULL, // current Directory
		&si,	//LPSTARTUPINFOW
		&pi	//LPPROCESS_INFORMATION
	)){
		printf("(<_<) failed to create process, error; %ld", GetLastError());
		return EXIT_FAILURE;
	}

	printf("(^_^) process started! pid: %ld",pi.dwProcessId);

	while(1){
		printf("(-_-) window x: %i\n",si.dwXSize);
		Sleep(1);
	}

	return EXIT_SUCCESS;
}
