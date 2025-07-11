#include <windows.h>
#include "FileSys.h"
#include <tlhelp32.h>

const char * GOOD = "[+]";
const char * BAD = "[-]";
const char * WAIT = "[*]";

const bool hasHNDInheritance = false;
const unsigned int emptyValue = NULL;

using namespace std;

#ifndef PCLASS_H
#define PCLASS_H


class Buffer{
      private:
    SIZE_T memoryWriteSize; //memory size
    LPCVOID memBuffer;      //type of memory // pointer to typedef(any memory type [down]))
      public:

      Buffer(){
      memoryWriteSize = 0;
      memBuffer = 0;
      }


    void setMemoryWriteSize(SIZE_T memSpecy){memoryWriteSize = memSpecy;}
    void setBuffer(LPCVOID buffer){memBuffer = buffer;}
    SIZE_T getMemoryWriteSize(){return memoryWriteSize;}
    LPCVOID getBuffer(){return memBuffer;}

};



class Process: public Buffer{
    private:
//process & memorywrite information
    DWORD PID;              //process id
    HANDLE hProcess;        //process handle

    public:                                                     
Process(): Buffer() {    
    PID = 0;
    hProcess = NULL;
	}

Process(const char * processName){
    PID = findProcessID(processName);

    if(PID == emptyValue){
        cout << '\n' << BAD << "Cannot find Process ID: " << GetLastError();
    }

    WaitForSingleObject(hProcess, 500);
    hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE, hasHNDInheritance, PID);

    if(hProcess == INVALID_HANDLE_VALUE){
        cout << '\n' << BAD << " Handle error: " << GetLastError();
    }else{
        cout << '\n' << GOOD << " Handle: " << hProcess << endl;
    }

}

    ~Process(){
        cout << '\n' << WAIT << " closing handle... ";
        CloseHandle(hProcess);
    }

    //y-suffix to denote base variable operation

    void setProcessID(DWORD Piddy){PID = Piddy;}
    void setProcessHandle(HANDLE PHandy){hProcess = PHandy;}
    DWORD getProcessID(){return PID;}
    HANDLE getProcessHandle(){return hProcess;}


        DWORD findProcessID(const char* fileName){
        PROCESSENTRY32 pe32;
        const int tempPID = {0};
        pe32.dwSize = sizeof(PROCESSENTRY32);

        
        WaitForSingleObject(hProcess, 1000);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, tempPID);
        

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

                    //remove later lol

                    return pe32.th32ProcessID;
                }
            }while(Process32Next(hSnapshot, &pe32));
        }                                       
        //returning empty PID
        return emptyValue;
    }

        


    bool readProcessMemory(HANDLE process, LPVOID buffer, SIZE_T bufferSize){return ReadProcessMemory(process, NULL, buffer, bufferSize, NULL);}
    bool writeProcessMemory(HANDLE process, LPCVOID buffer, SIZE_T bufferSize ){return WriteProcessMemory(process, NULL, buffer, bufferSize, NULL);}
    

wwwww
};



#endif
