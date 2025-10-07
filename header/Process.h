#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "FileSys.h"
#include <tlhelp32.h>

const char * GOOD = "[+]";
const char * BAD = "[-]";
const char * WAIT = "[*]";

const bool HNDInheritable = false;
const unsigned int fail = -1;

using namespace std;

#ifndef PCLASS_H
#define PCLASS_H

class Buffer{
      private:
	      SIZE_T memSize; //memory size
	      LPVOID memBuff;     //type of memory // pointer to typedef(any memory type [down]))
      public:

      Buffer(){
	      memSize = 0;
	      memBuff = 0;
      }

    void setMemSize(SIZE_T memSize){this->memSize = memSize;}
    void setMemBuff(LPVOID membuff){this->memBuff = memBuff;}
    SIZE_T getMemSize(){return memSize;}
    LPVOID getMemBuff(){return memBuff;}
};



class Process: public Buffer : public FileSys{
    private:
//process & memorywrite information
    DWORD PID;              //process id
    HANDLE hProcess;        //process handle

    public:                                                     
    Process(): Buffer() 
    {    
        PID = 0;
	hProcess = NULL;
    }

Process(const char * processName){
    PID = findProcessID(processName);

    if(PID == fail){
        cout << '\n' << BAD << "Cannot find Process ID: " << GetLastError();
    }

    WaitForSingleObject(hProcess, 500);

    hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE, HNDInheritable, PID);

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


    DWORD findProcessID(const char * fileName)
    {
        PROCESSENTRY32 pe32;
        const int tempPID = {0};
        pe32.dwSize = sizeof(PROCESSENTRY32);

        
        WaitForSingleObject(hProcess, 1000);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, tempPID);
        

        if(hSnapshot == INVALID_HANDLE_VALUE)
	{

            cout<< '\n' <<  BAD << " cannot find handle: " << GetLastError();
            exit(EXIT_FAILURE);
        }

        cout << '\n' << WAIT << " Finding Process ID...";
                                                            
        if(Process32First(hSnapshot, &pe32))
	{

            do
	    {
                if(strcmp(pe32.szExeFile, fileName) == 0)
		{
                    CloseHandle(hSnapshot);

                    cout  << '\n' << GOOD << " Found PID: " << pe32.th32ProcessID;
                    return pe32.th32ProcessID;
                }
            }while(Process32Next(hSnapshot, &pe32));
        }                                       
        //returning empty PID
        return fail;
    }




    void setPID(DWORD PID){this->PID = PID;}
    void setHProcess(HANDLE hProcess){this->hProcess = hProcess;}
    DWORD getPID(){return PID;}
    HANDLE getHProcess(){return hProcess;}
        


    bool readMem(HANDLE hProcess, Buffer payload)
	{
		return ReadProcessMemory(hProcess, NULL, payload.getMemBuff(), payload.getMemSize(), NULL);
	}
    bool writeMem(HANDLE hProcess, Buffer payload)
	{
		return WriteProcessMemory(hProcess, NULL, payload.getMemBuff(), payload.getMemSize(), NULL);
	}
    


};



#endif
