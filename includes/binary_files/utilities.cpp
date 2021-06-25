/**
 * @file utilities.cpp
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief Utilities for opening binary files
 * @version 0.1
 * @date 2021-06-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "./utilities.h"
#include <iostream>
#include <fstream>

using namespace std;

bool file_exists(const char filename[])
{
    fstream f;
    f.open(filename, std::fstream::in | std::fstream::binary);
    if (f.fail()) { return false; }
    f.close();
    return true;
}

void open_fileRW(fstream& f, const char filename[])
{
    if (! file_exists(filename)) // if we create a new file, give it write access only
    {
        f.open(filename, std::fstream::out | std::fstream::binary);
        if (f.fail())
        {
            cout << "Failed when attempting to create [" << filename << "] in RW mode" << endl;
        }
    }
    else
    {
        f.open (filename, std::fstream::in | std::fstream::out | std::fstream::binary);
        if (f.fail())
        {
            cout << "Failed when attempting to open [" << filename << "] in RW mode" << endl;
        }
    }
}

void open_fileW(fstream& f, const char filename[])
{
    f.open(filename, std::fstream::out | std::fstream::binary);
    if (f.fail())
    {
        cout << "Failed when attempting to open [" << filename << "] in W mode" << endl;
    }
}