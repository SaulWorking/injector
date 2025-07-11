#include <filesystem>
#include <iostream>
#include <fstream>

using namespace std;

#ifndef FILE_H
#define FILE_H

class File {
    private:
    
    char * fileName;
    filesystem::path filePath;
    filesystem::path userPath;

    public:

filesystem::path findFile(const char * fileName){
    
    userPath = filesystem::current_path();
    filePath = userPath / fileName;

    return filePath;
}







};

#endif 
