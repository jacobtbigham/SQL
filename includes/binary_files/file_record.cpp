/**
 * @file file_record.cpp
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief FileRecord implementation
 * @version 0.1
 * @date 2021-06-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "./file_record.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

long FileRecord::num_records = 0;

FileRecord::FileRecord()
{
    recno = -1;
    _record[0][0] = '\0'; // null byte represents the end of data
}

FileRecord::FileRecord(const vector<string>& v)
{
    int items_in_record = (v.size() < MAX) ? v.size() : MAX;
    for (int rec = 0; rec < items_in_record; ++rec)
    {
        string curr = v.at(rec);
        int curr_size = (curr.size() < MAX) ? curr.size() : MAX;
        for (int i = 0; i < (MAX - curr_size); ++i) // pad with spaces
        {
            _record[rec][i] = ' ';
        }
        for (int i = 0; i < curr_size; i++) // write data to record
        {
            _record[rec][i + (MAX - curr_size)] = curr.at(i);
        }
    }
    if (v.size() < MAX) // indicate end of record
    {
        _record[v.size()][0] = '\0';
    }
    recno = num_records++;
}

int FileRecord::write(fstream& outs)
{
    outs.seekg(0, outs.end);
    for (int i = 0; i < MAX; ++i)
    {
        outs.write(_record[i], sizeof(_record[i]));
    }
    return recno;
}

long FileRecord::read(fstream& ins, long _recno)
{
    long pos = _recno * MAX * MAX;
    ins.seekg(0, ins.end);
    long size = ins.tellg();
    if (pos >= size) { return 0; } // reached the end of the file
    ins.seekg(pos, ios_base::beg);
    int items_in_record = 0;
    for (int i = 0; i < MAX; ++i)
    {
        string next_in = "";
        char next;
        for (int j = 0; j < MAX; ++j)
        {
            ins.get(next);
            next_in += next;
        }
        if (next_in.at(0) == '\0')
        {
            break;
        }
        info.push_back(next_in);
        strncpy(_record[i], next_in.c_str(), MAX);
        ++items_in_record;
    }
    return MAX * items_in_record;
}


ostream& operator<<(ostream& outs, const FileRecord& print_me)
{
    for (int i = 0; i < print_me.MAX; ++i)
    {
        if (print_me._record[i][0] == '\0') { break; }
        for (int j = 0; j < print_me.MAX; j++)
        {
            outs << print_me._record[i][j];
        }
    }
    return outs;
}