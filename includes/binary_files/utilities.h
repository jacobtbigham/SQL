#ifndef UTILITIES_H
#define UTILITIES_H

/**
 * @file utilities.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief Utilities for opening binary files
 * @version 0.1
 * @date 2021-06-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include <fstream>

using namespace std;

// FUNCTIONS IN THE PUBLIC INTERFACE:
// bool file_exists(const char filename[])
// void open_fileRW(fstream& f, const char filename[])
// void open_fileW(fstream& f, const char filename[])

/**
 * @brief Returns whether a file exists
 * 
 * @param filename the filename
 * @return true a file with the specified name exists
 * @return false a file with the specificed name does not exist
 */
bool file_exists(const char filename[]);

/**
 * @brief Open a file for read and write access (if creating a new file, write only)
 * 
 * @param f the filestream to open the file into
 * @param filename the filename
 */
void open_fileRW(fstream& f, const char filename[]);

/**
 * @brief Open a file for write access only
 * 
 * @param f the filestream to open the file into
 * @param filename 
 */
void open_fileW(fstream& f, const char filename[]);

#endif