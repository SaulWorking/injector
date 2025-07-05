#include <windows.h>
#include <tlhelp32.h>

using namespace std;

#ifndef PCLASS_H
#define PCLASS_H

class Process {
    private:
//process information
    HWND hWnd; //window handle
    DWORD PID; //process id
    HANDLE hProcess; //process handle
//buffer information
    SIZE_T memoryWriteSize; //memory size
    LPCVOID memBuffer; //type of memory // pointer to typedef(any memory type [down]))
    public:                                                     
//constructor for if nothing happens 
 Process(){    
    hWnd = NULL;
    PID = 0;
    hProcess = NULL;

    memBuffer = NULL;
    memoryWriteSize = 0;
	}
    
 Process(const wchar_t* processName){



//second error checker

 if(processName != NULL || strlen(processName) > 0){
	cout << "\n process name: " << processName << endl;
	
	
	
	if(hProcess == NULL{throw GetLastError();}


	PROCESSSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSSENTRY32);
	

	HANDLE hSnapshot  = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
// ^^^^^^^^^^^^^^^^^^ add on
// ask help for pe32 finding a process using processfilename	


	hWnd = FindWindow(NULL, processName)

	PID = GetWindowThreadProcessID(hWnd, NULL);
	hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, false, PID);

	
	if(hProcess == NULL){            
	cout << "\nUnable to get a handle to the process:" << endl;
	throw exception(GetLastError());
	} 




}

    //reliant on memory or somethign
    ~Process(){CloseHandle(hProcess);}

    

    //y-suffix to denote base variable operation

    void setMemoryWriteSize(SIZE_T memSpecy){memoryWriteSize = memSpecy;}
    void setWindowHandle(HWND WHandy ){hWnd = WHandy;}
    void setProcessID(DWORD Piddy){PID = Piddy;}
    void setProcessHandle(HANDLE PHandy){hProcess = PHandy;}


    SIZE_T getMemoryWriteSize(){return memoryWriteSize;}
    HWND getWindowHandle(){return hWnd;}
    DWORD getProcessID(){return PID;}
    HANDLE getProcessHandle(){return hProcess;}


    bool writeProcessMemory(HANDLE process, LPCVOID buffer, SIZE_T bufferSize ){
        return WriteProcessMemory(process, NULL, buffer, bufferSize, NULL);
    }
    


};



#endif
