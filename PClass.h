#include <windows.h>
#include <tlhelp32.h>

const char * GOOD = "[+]";
const char * BAD = "[-]";
const char * WAIT = "[*]";

using namespace std;

#ifndef PCLASS_H
#define PCLASS_H

class Process {
    private:
//process information
    DWORD PID;              //process id
    HANDLE hProcess;        //process handle
//buffer information
    SIZE_T memoryWriteSize; //memory size
    LPCVOID memBuffer;      //type of memory // pointer to typedef(any memory type [down]))
    public:                                                     
//constructor for if nothing happens 
Process(){    
    PID = 0;
    hProcess = NULL;

    memBuffer = NULL;
    memoryWriteSize = 0;
	}

Process(const char * processName){
    PID = findProcessID(processName);

    hProcess = OpenProcess(PROCESS_VM_READ, false, PID);

    if(hProcess == INVALID_HANDLE_VALUE){
        cout << '\n' << BAD << " Handle error: " << GetLastError();

    }else{
        cout << '\n' << GOOD << " Handle: " << hProcess << endl;
    }

}
    //reliant on memory or somethign
    ~Process(){
        cout << '\n' << WAIT << " closing handle... ";
        CloseHandle(hProcess);
    }
 
    //y-suffix to denote base variable operation

    void setMemoryWriteSize(SIZE_T memSpecy){memoryWriteSize = memSpecy;}
    void setProcessID(DWORD Piddy){PID = Piddy;}
    void setProcessHandle(HANDLE PHandy){hProcess = PHandy;}

    DWORD findProcessID(const char* fileName){
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if(hSnapshot == INVALID_HANDLE_VALUE){
            cout<< '\n' <<   BAD << " Crazy: " << GetLastError();
            exit(EXIT_FAILURE);
        }

        cout << '\n' << WAIT << " Finding Process ID...";

                                                            
        if(Process32First(hSnapshot, &pe32)){
            do{
                if(strcmp(pe32.szExeFile, fileName) == 0){
                    CloseHandle(hSnapshot);

                    cout  << '\n' << GOOD << " Found PID: " << pe32.th32ProcessID;

                		MessageBoxA(NULL, "open roblock pls", "Penis", wMB_OK);
                    return pe32.th32ProcessID;
                }
            }while(Process32Next(hSnapshot, &pe32));
        }                                       
        //returning empty PID
        return 1;
    }


    SIZE_T getMemoryWriteSize(){return memoryWriteSize;}
    DWORD getProcessID(){return PID;}
    HANDLE getProcessHandle(){return hProcess;}

    bool readProcessMemory(HANDLE process, LPVOID buffer, SIZE_T bufferSize){return ReadProcessMemory(process, NULL, buffer, bufferSize, NULL);}
    bool writeProcessMemory(HANDLE process, LPCVOID buffer, SIZE_T bufferSize ){return WriteProcessMemory(process, NULL, buffer, bufferSize, NULL);}
    


};



#endif
