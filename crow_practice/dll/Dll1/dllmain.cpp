// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule, //handle to the DLL module
                       DWORD  ul_reason_for_call,   //reason for calling
                       LPVOID lpReserved    //what is reserved?
                     )
{
    
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        MessageBoxW(NULL, L"KAW KAW KAW", L"WHO GOES THERE", MB_ICONQUESTION | MB_OK);
        //initialize once for every new process (load into virtual address space)
        //return false to fail DLL load
        break;
    case DLL_THREAD_ATTACH:
        //do thread-specific initialization
        break;
    case DLL_THREAD_DETACH:
        //do thread-specific cleanup
        break;
    case DLL_PROCESS_DETACH:
        //do process-specific cleanup
        if(lpReserved != nullptr)   //do not clean up if process termination scenario
        {
            break;
        }
        //cleanup
        break;
    }
    return TRUE;
}

