#include <windows.h>
#include <stdio.h>


int main(void){

struct _STARTUPINFOW si = { 0 };
struct _PROCESS_INFORMATION pi = { 0 };

const wchar_t* app_path = L"C:\\Windows\\System32\\notepad.exe";	//application path


printf("(<_<) Trying to start process: %ld\n",app_path);

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
	
		printf("(<_<) process couldn't be created, %ld\n", GetLastError());	
	}	

printf("(^_^) process has been created successfully)\n");
printf("==================================================================\n");
printf("(tung) PID: %ld\n",pi.dwProcessId);
printf("(tung) TID: %ld\n",pi.dwThreadId);
printf("(tung) Process Handle: %p\n",pi.hProcess);
printf("(tung) Thread Handle: %p\n",pi.hThread);
printf("==================================================================\n");


switch(WaitForSingleObject(pi.hProcess,1000)){

	case WAIT_ABANDONED:
	break;

	case WAIT_OBJECT_0:
	break;

	case WAIT_TIMEOUT:
	printf("(bye) The process is closing..");
	if(CloseHandle(pi.hProcess) || CloseHandle(pi.hThread))
		printf("(^_^) atleast one handle closed successfully");
	TerminateProcess(pi.hProcess,0);
	break;

	case WAIT_FAILED:
	break;
	default:
	printf("(bye) idk nothing happened");
}


	return EXIT_SUCCESS;
}
