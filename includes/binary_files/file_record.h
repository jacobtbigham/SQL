#ifndef FILE_RECORD_H
#define FILE_RECORD_H

/**
 * @file file_record.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief Reads and writes data to binary files for later retrieval
 * @version 0.1
 * @date 2021-06-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

/**
 * This structure gives the ability to write and read data records to file.
 * 
 * To write a record (e.g., ["Jacob", "Bigham", 2021, "UCLA"]) to file, first
 * create a FileRecord from a vector<string> containing the record information.
 * Then, use the write() function to write that record to whatever fstream in
 * which you want to house that information (namely, whichever file holds the
 * data for the table to which you want to add the record).
 * 
 * The read a record from file, use the read function.
 * 
 * Write will return a record number, and read will require a record number.
 * The record number is the position in the data file at which a record resides.
 * You will need to keep track of these to usefully read or write data.
 * 
 * Because the record number is a static variable within the FileRecord struct,
 * you need to reset the record number (call reset_record_number()) each time
 * you process a new table/data file.
 * 
 * By default, the maximum number of values allowed in a record (and the maximum
 * length for each value, which is controlled by the same/one variable) is 50.
 * There is no range checking for this: data that goes beyond this limit is truncated.
 * If you need more space, adjust the MAX variable.
 * 
 */

// FUNCTIONS IN THE PUBLIC INTERFACE:
// FileRecord()
// FileRecord(const vector<string>& v)
// int write(fstream& outs)
// long read(fstream& ins, long recno)
// vector<string> get_info()
// static void reset_record_number()
// friend ostream& operator<<(ostream& outs, const FileRecord& print_me)

struct FileRecord
{
    public:
        /**
         * @brief Construct a new File Record object
         * 
         */
        FileRecord();

        /**
         * @brief Construct a new File Record object from a vector of strings containing 
         * 
         * @param v 
         */
        FileRecord(const vector<string>& v);

        /**
         * @brief Write the info of the record to file and return the record number
         * 
         * @param outs the fstream to write to
         * @return int the record number
         */
        int write(fstream& outs);

        /**
         * @brief Read the given record number from file and return the numnber of bytes read in
         * 
         * @param ins the fstream to read from
         * @param recno the record number
         * @return long the number of bytes read in
         */
        long read(fstream& ins, long recno);

        /**
         * @brief Get the data the FileRecord has read or written (empty if empty)
         * 
         * @return vector<string> the data in the FileRecord
         */
        vector<string> get_info() { return info; }

        /**
         * @brief Reset the record number (use when moving to a different table)
         * 
         */
        static void reset_record_number() { num_records = -1; }

        /**
         * @brief Print the FileRecord's information to the indicated output stream
         * 
         * @param outs the output stream to print to
         * @param print_me the FileRecord to print
         * @return ostream& the output stream printed to
         */
        friend ostream& operator<<(ostream& outs, const FileRecord& print_me);

    private:
        static const int MAX = 50; // the maximum number of entries allowed
                                   // also the maximum length for entries
        char _record[MAX][MAX] = {'\0'}; // an array that stores the data of the record
        static long num_records; // the total number of records
        long recno; // the record number for this FileRecord
        vector<string> info; // the data stored in the FileRecord
};

#endif