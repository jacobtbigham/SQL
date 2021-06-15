#ifndef _LINKED_LIST_FUNCTIONS_H
#define _LINKED_LIST_FUNCTIONS_H

/**
 * @file linked_list_functions.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief Linked list functionality for the Node class (intended for sequential access)
 * @version 0.1
 * @date 2021-06-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../node/node.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

// THIS IS NOT INTENDED TO BE USED AS A LINKED LIST CLASS
// These are utility functions from which Stack and Queue classes are built

// FUNCTIONS IN THIS FILE:

template <typename T>
void print(node<T>* head, std::string end = "|||");

template <typename T>
void print_reverse(node<T>* head, std::string end = "|||");

template <typename T>
node<T>* search(node<T>* head, T key);

template <typename T>
node<T>* prepend(node<T>* &head, T insert_me);

template <typename T>
node<T>* insert_after(node<T>* &head, node<T>* after_me, T insert_me);

template <typename T>
node<T>* insert_before(node<T>*& head, node<T>* before_me, T insert_me);

template <typename T>
node<T>* node_before(node<T>* before_this);

template <typename T>
T remove(node<T>*& head, node<T>* remove_me);

template <typename T>
node<T>* duplicate(node<T>* head);

template <typename T>
void clear(node<T>*& head);

template <typename T>
T& at(node<T>* head, int pos);


/**
 * @brief Print the list to cout
 * 
 * @tparam T the type of data stored in each node of the linked list
 * @param head the head node of the linked list
 * @param end the string to terminate the list
 */
template <typename T>
void print(node<T>* head, std::string end)
{

    node<T>* traverser = head;
    while (traverser != nullptr)
    {
        std::cout << *traverser;
        traverser = traverser->_next;
    }
    std::cout << end;
}


/**
 * @brief Print the list in reverse to cout
 * 
 * @tparam T the type of data stored in each node of the linked list
 * @param head the head node of the linked list (printed last)
 * @param sep the string to terminate the list
 */
template <typename T> 
void print_reverse(node<T>* head, std::string end)
{
    if (head == nullptr)
    {
        std::cout << end;
    }
    else
    {
        print_reverse_helper(head);
        std::cout << end;
    }
}

template <typename T> 
void print_reverse_helper(node<T> *head)
{
    if (head->_next == nullptr)
    {
        std::cout << *head;
    }
    else
    {
        print_reverse_helper(head->_next);
        std::cout << *head;
    }
}


/**
 * @brief Search for a value in a linked list
 * 
 * @tparam T the type of data stored in each node of the linked list
 * @param head the head node of the linked list
 * @param key the value to search for
 * @return node<T>* the node containing key, or nullptr if the list
 *         does not contain the key
 */
template <typename T>
node<T>* search(node<T>* head, T key)
{
    node<T>* traverser = head;
    while (traverser != nullptr)
    {
        if (traverser->_item == key)
        {
            return traverser;
        }
        traverser = traverser->_next;
    }
    return nullptr;
}


/**
 * @brief Insert a new node at the beginning of a linked list
 * 
 * @tparam T the type of data stored in each node of the linked list
 * @param head the head node of the linked list
 * @param insert_me the value to add to the beginning of the list
 * @return node<T>* the new head of the linked list
 */
template <typename T>
node<T>* prepend(node<T>* &head, T insert_me)
{
    node<T>* new_head = new node<T>;
    new_head->_item = insert_me;
    new_head->_next = head;
    if (head != nullptr)
    {
        head->_prev = new_head;
    }
    head = new_head;
    return head;
}


/**
 * @brief Insert a new item after a given node
 * 
 * @tparam T the type of data stored in each node of the linked list
 * @param head the head node of the linked list
 * @param after_me the node to insert after
 * @param insert_me the item to insert
 * @pre head is not nullptr
 * @pre after_me is not nullptr and is in the list
 * @return node<T>* the newly inserted node
 */
template <typename T>
node<T>* insert_after(node<T>* &head, node<T>* after_me, T insert_me)
{
    assert(head != nullptr);
    assert(after_me != nullptr);
    node<T>* insert_node = new node<T>;
    insert_node->_item = insert_me;
    insert_node->_prev = after_me;
    if (after_me->_next == nullptr) // inserting at the end
    {
        after_me->_next = insert_node;
        return insert_node;
    }
    else // inserting in the middle
    {
        node<T>* old_after = after_me->_next;
        after_me->_next = insert_node;
        insert_node->_next = old_after;
        old_after->_prev = insert_node;
        return insert_node;
    }
}


/**
 * @brief Insert a new item before a given node
 * 
 * @tparam T the type of data stored in each node of the linked list
 * @param head the head node of the linked list
 * @param before_me the node to insert before
 * @param insert_me the item to insert
 * @pre head is not nullptr
 * @pre before_me is not nullptr and is in the list
 * @return node<T>* the newly inserted node
 */
template <typename T> //DONE
node<T>* insert_before(node<T>*& head, node<T>* before_me, T insert_me)
{
    assert(head != nullptr);
    assert(before_me != nullptr);
    if (head == before_me) // inserting at the beginning
    {
        return prepend(head, insert_me);
    }
    else // inserting in the middle
    {
        node<T>* old_prev = before_me->_prev;
        node<T>* insert_node = new node<T>;
        insert_node->_item = insert_me;
        insert_node->_next = before_me;
        insert_node->_prev = old_prev;
        before_me->_prev = insert_node;
        old_prev->_next = insert_node;
        return insert_node;
    }
}

/**
 * @brief Return node before a given node in a linked list
 * 
 * @tparam T the type of data stored in each node of the linked list
 * @param before_this the node whose previous node is returned
 * @return node<T>* the node before before_this, or nullptr if before_this is nullptr
 */
template <typename T>
node<T>* node_before(node<T>* before_this)
{
    if (before_this == nullptr) { return nullptr; }
    return before_this->_prev;
}


/**
 * @brief Delete a node from a linked list
 * 
 * @tparam T the type of data stored in each node of the linked list
 * @param head the head node of the linked list
 * @param remove_me the node to delete
 * @pre remove_me is not nullptr and is in the list
 * @pre head is not nullptr
 * @return T the item in the deleted node
 */
template <typename T>
T remove(node<T>*& head, node<T>* remove_me)
{
    assert(head != nullptr);
    assert(remove_me != nullptr);
    if (head == remove_me)
    {
        if (head->_next == nullptr)
        {
            T item = head->_item;
            delete head;
            head = nullptr;
            return item;
        }
        node<T>* old_head = head;
        T old_head_item = old_head->_item;
        head = old_head->_next;
        head->_prev = nullptr;
        delete old_head;
        return old_head_item;
    }
    node<T>* next = remove_me->_next;
    node<T>* prev = remove_me->_prev;
    T removed_item = remove_me->_item;
    delete remove_me;
    if (prev != nullptr)
    {
        prev->_next = next;
    }
    return removed_item;
}


/**
 * @brief Copy the list
 * 
 * @tparam T the type of data stored in each node of the linked list
 * @param head the head node of the linked list
 * @return node<T>* the head node of the copy
 */
template <typename T>
node<T>* duplicate(node<T>* head)
{
    if (head == nullptr)
    {
        return nullptr;
    }
    node<T>* new_head = new node<T>;
    new_head->_item = head->_item;
    node<T>* new_traverser = new_head;
    node<T>* old_traverser = head;
    while (old_traverser->_next != nullptr)
    {
        node<T>* next = new node<T>;
        next->_prev = old_traverser;
        next->_item = old_traverser->_next->_item;
        new_traverser->_next = next;
        new_traverser = new_traverser->_next;
        old_traverser = old_traverser->_next;
    }
    new_traverser->_next = nullptr;
    return new_head;
}


/**
 * @brief Clear the linked list
 * 
 * @tparam T the type of data stored in each node of the linked list
 * @param head the head node of the linked list
 */
template <typename T> //DONE
void clear(node<T>*& head)
{
    while (head != nullptr)
    {
        node<T>* old_head = head;
        head = head->_next;
        delete old_head;
    }
}


/**
 * @brief Access an item in the linked list by index
 * 
 * @tparam T the type of data stored in each node of the linked list
 * @param head the head node of the linked list
 * @param pos the index being accessed
 * @pre pos is a valid index
 * @return T& the value at index pos
 */
template <typename T>
T& at(node<T>* head, int pos)
{
    int curr_pos = 0;
    while (head != nullptr && curr_pos != pos)
    {
        head = head->_next;
        ++curr_pos;
    }
    if (curr_pos == pos)
    {
        return &(head->_item);
    }
    // if here, out of bounds
    assert("call to at(pos): pos outside of bounds in linked list access" == "");
    return T(); //to compile
}

#endif