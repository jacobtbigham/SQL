#include "./utilities.h"
#include <iostream>
#include <fstream>
using namespace std;

bool file_exists(const char filename[])
{
    fstream ff;
    ff.open(filename, std::fstream::in | std::fstream::binary);
    if (ff.fail()) { return false; }
    ff.close();
    return true;
}

void open_fileRW(fstream& f, const char filename[])
{
    if (!file_exists(filename)){
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail())
        {
            cout << "file open (RW) failed: with out|" << filename << "]" << endl;
            return;
        }
    }
    else{
        f.open (filename, std::fstream::in | std::fstream::out| std::fstream::binary);
        if (f.fail()){
            cout << "file open (RW) failed. [" << filename<<"]" << endl;
            return;
        }
    }
}

void open_fileW(fstream& f, const char filename[])
{
    f.open(filename, std::fstream::out| std::fstream::binary);
    if (f.fail())
    {
        cout << "file open failed: " << filename << endl;
        return;
    }
}