#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#ifndef FILE_H
#define FILE_H

class File {
    private:
    
    string fileName;
    filesystem::path filePath;
    filesystem::path curDir;

    public:
	
    File()
    {
	fileName = "
    }

	filesystem::path findFile(string fileName)
	{
	    curDir = filesystem::current_path();
	    filePath = curDir / fileName;
	    return filePath;
	}







};

#endif 
