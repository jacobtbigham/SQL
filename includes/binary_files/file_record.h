#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
using namespace std;

struct FileRecord
{
    private:
        static const int MAX = 50;
        static long num_records;
        long recno;

    public:
        FileRecord()
        {
            recno = -1;
            _record[0][0] = '\0'; // null byte represents the end of data
        }

        FileRecord(const vector<string>& v)
        {
            for (int i = 0; i < v.size(); ++i)
            {
                string curr = v.at(i);
                int start = 0;
                if (curr.size() < MAX)
                {
                    for (int j = 0; j < (MAX - curr.size()); ++j)
                    {
                        _record[i][j] = ' ';
                        ++start;
                    }
                }
                for (int j = 0; j < curr.size(); j++)
                {
                    if (start >= MAX) { break; }
                    _record[i][start] = curr.at(j);
                    ++start;
                }
            }
            if (v.size() < MAX)
            {
                _record[v.size()][0] = '\0';
            }
            recno = num_records++;
        }

        static void reset_record_number() { num_records = -1; }

        vector<string> get_info()
        {
            return info;
        }

        int write(fstream& outs);

        long read(fstream& ins, long recno);

        friend ostream& operator<<(ostream& outs, const FileRecord& print_me);

        char _record[MAX][MAX] = {'\0'};

        vector<string> info;
};

#endif