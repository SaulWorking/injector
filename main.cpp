#include <iostream>
#include "Process.h"
#include "FileSys.h"
using namespace std;


//requirement -> inject into a roblox game
/*first, find how to inject into a roblox game.
    then, we go through the process.
    Then, we inject.
    Then it loops and waits for text processes or something
    until user input says no

    microsfot in generla
    https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-writeprocessmemory


    how to get process info
//https://learn.microsoft.com/en-us/windows/win32/procthread/process-handles-and-identifiers

roblox exploiting tiny
    https://devforum.roblox.com/t/where-does-an-exploit-inject-to/760315
    ^^?
linus process injection if you wajnt
    https://www.akamai.com/blog/security-research/the-definitive-guide-to-linux-process-injection
*/
//https://stackoverflow.com/questions/24182927/c-writeprocessmemory-how-to-get-the-right-lpbaseaddress
/*
//https://stackoverflow.com/questions/31072800/openprocess-vs-createprocess
*/

/*

https://sensepost.com/blog/2019/hacking-doom-for-fun-health-and-ammo/


*/


int main(void){

Process openProcess("RobloxPlayerBeta.exe");

File fileSystem;

openProcess.~Process();
    return 0;
}
