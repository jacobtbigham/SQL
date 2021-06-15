#ifndef B_TREE_ARRAY_FUNCS_H
#define B_TREE_ARRAY_FUNCS_H

using namespace std;
#include <iostream>
#include <vector>

template <class T>
T maximal(const T& a, const T& b)                      //return the larger of the two items
{
    return (a > b ? a : b);
}

template <class T>
void swap(T& a, T& b)  //swap the two items
{
    T temp = a;
    a = b;
    b = temp;
}

template <class T>
int index_of_maximal(T data[ ], int n)                 //return index of the largest item in data
{
    if (n == 0) { return 0; }
    T largest = data[0];
    int idx = 0;
    for (int i = 1; i < n; ++i)
    {
        int curr = data[i];
        if (curr > largest)
        {
            largest = curr;
            idx = i;
        }
    }
    return idx;
}

template <class T>
void ordered_insert(T data[ ], int& n, T entry)        //insert entry into the sorted array
                                                        //data with length n
{
    int insert_idx = 0;
    while (insert_idx < n && data[insert_idx] < entry) { ++insert_idx; }
    for (int i = n; n > insert_idx; --n)
    {
        data[i] = data[i-1];
    }
    data[insert_idx] = entry;
    n++;
}                                                        

template <class T>
int first_ge(const T data[ ], int n, const T& entry)   //return the first element in data that is
                                                        //not less than entry
{
    if (n == 0) { return 0; }
    for (int i = 0; i < n; ++i)
    {
        if (!(data[i] < entry))
        {
            return i;
        }
    }
    return n;
}                                                

template <class T>
void attach_item(T data[ ], int& n, const T& entry)    //append entry to the right of data
{
    data[n++] = entry;
}

template <class T>
void insert_item(T data[ ], int i, int& n, T entry)    //insert entry at index i in data
{
    if (n == 0 && i == 0)
    {
        data[0] = entry;
        ++n;
        return;
    }
    for (int j = n; j > i; --j)
    {
        data[j] = data[j-1];
    }
    data[i] = entry;
    n++;
}

template <class T>
void detach_item(T data[ ], int& n, T& entry)          //remove the last element in data and place
                                                        //it in entry
{
    entry = data[--n];
}                                                        

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry)   //delete item at index i and place it in entry
{
    if (i == 0 && n == 1)
    {
        entry = data[0];
        n = n-1;
        data[0] = data[1];
        return;
    }
    entry = data[i];
    for (int j = i; j < n-1; ++j)
    {
        data[j] = data[j+1];
    }
    n = n-1;
}

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2)   //append data2 to the right of data1
{
    for (int i = 0; i < n2; ++i)
    {
        data1[n1 + i] = data2[i];
    }
    n1 += n2;
}

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2)   //move n/2 elements from the right of data1
                                                        //and move to data2
{
    int half = n1 / 2;
    int i = 0;
    while (half < n1)
    {
        data2[n2 + i] = data1[half];
        ++i;
        ++half;
    }
    n1 /= 2;
    n2 += i;
}

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size)              //copy src[] into dest[]
{
    for (int i = 0; i < src_size; ++i)
    {
        dest[i] = src[i];
    }
    dest_size = src_size;
}

template <class T>
void print_array(const T data[], int n, int pos = -1)  //print array data
{
    cout << "[";
    for (int i = 0; i < n - 1; ++i)
    {
        cout << data[i] << " ";
    }
    cout << data[n - 1] << "]";
}

template <class T>
bool is_gt(const T data[], int n, const T& item)       //item > all data[i]
{
    for (int i = 0; i < n; ++i)
    {
        if (!(data[i] < item)) { return false; }
    }
    return true;
}

template <class T>
bool is_le(const T data[], int n, const T& item)       //item <= all data[i]
{
    for (int i = 0; i < n; ++i)
    {
        if (data[i] < item) { return false; }
    }
    return true;
}

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list) //print vector list
{
    if (list.size() == 0)
    {
        outs << "[]";
        return outs;
    }
    outs << "[";
    for(int i = 0, size = list.size(); i < size - 1; ++i)
    {
        outs << list.at(i) << ", ";
    }
    outs << list.back() << "]";
    return outs;
}

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme) //list.push_back addme
{
    list.push_back(addme);
    return list;
}

#endif
