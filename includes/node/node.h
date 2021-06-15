#ifndef _NODE_H
#define _NODE_H

/**
 * @file node.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief Node struct for linked-list style data structures
 * @version 0.1
 * @date 2021-06-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>

using namespace std;

template <class T>
struct node{
    T _item;
    node<T>* _next;
    node<T>* _prev;

    /**
     * @brief Construct a new node object
     * 
     * @param item the value stored in the node
     * @param next a pointer to the subsequent node
     * @param prev a pointer to the preceding node
     */
    node(const T& item = T(), node<T>* next = NULL, node<T>* prev = NULL)
    {
        _item = item;
        _next = next;
        _prev = prev;
    }

    /**
     * @brief Print the content of the node
     * 
     * @param outs the output stream to print to
     * @param print_me the node to print
     * @return ostream& the output stream printed to
     */
    friend ostream& operator << (ostream& outs, const node<T>& print_me)
    {
        outs << "[" << print_me._item << "]->";
        return outs;
    }
};

#endif