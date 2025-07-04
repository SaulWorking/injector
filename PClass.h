#include <windows.h>
using namespace std;

#ifndef PCLASS_H
#define PCLASS_H




class Process {

    private:


//process information
    HWND hWnd;
    DWORD PID;
    HANDLE hProcess;

//buffer information
    SIZE_T memoryWriteSize;
    // pointer to typedef(any memory type :3)
    //text later
    LPCVOID memBuffer;



    public:                                                     


    //probably for later
        
    Process(){    
    hWnd = NULL;
    PID = 0;
    hProcess = NULL;

    memBuffer = NULL;
    memoryWriteSize = 0;

        }

    Process(const char* processName){




        //reliant on an existing process(roblocky)

        if(processName != NULL || strlen(processName) > 0){
        
            CreateProcessA(
                processName,            //application name
                NULL,                   //cli command
                NULL,                   //process handle heritance (false)
                NULL,                   //process thread inheritance (false)
                false,                  // handl inheritance = false 
                NORMAL_PRIORITY_CLASS , //creation flags
                NULL,   //
                NULL, 
                NULL, 
                NULL);



            cout << "\n process name: " << processName << endl;


        hWnd = FindWindowA(NULL, processName);

        PID = GetWindowThreadProcessId(hWnd, NULL);
        hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, false, PID);

        }else{

            

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
