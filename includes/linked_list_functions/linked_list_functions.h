#ifndef _LINKED_LIST_FUNCTIONS_H
#define _LINKED_LIST_FUNCTIONS_H

/**
 * @file linked_list_functions.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../node/node.h"
#include <iostream>

using namespace std;

/**
 * @brief Print the list to cout
 * 
 * @tparam T the type of data stored in each node of the linked list
 * @param head the head node of the linked list
 * @param sep the string to separate node values 
 */
template <typename T>
void print(node<T>* head, std::string sep = "|||")
{

    node<T>* traverser = head;
    while (traverser != nullptr)
    {
        cout << *traverser;
        traverser = traverser->_next;
    }
    cout << sep;
}


/**
 * @brief Print the list in reverse to cout
 * 
 * @tparam T the type of data stored in each node of the linked list
 * @param head the head node of the linked list (printed last)
 * @param sep the string to separate node values
 */
template <typename T> 
void print_reverse(node<T>* head, std::string sep = "|||")
{
    if (head == nullptr)
    {
        cout << sep;
    }
    else
    {
        print_reverse_helper(head);
        cout << sep;
    }
}

template <typename T> 
void print_reverse_helper(node<T> *head)
{
    if (head->_next == nullptr)
    {
        cout << *head;
    }
    else
    {
        print_reverse_helper(head->_next);
        cout << *head;
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
 * @return node<T>* 
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

//insert after ptr: insert head if marker null
template <typename T>  //DONE
node<T>* _insert_after(node<T>* &head, node<T>* after_this, T insert_this)
{
    if (after_this == nullptr)
    {
        return _insert_head(head, insert_this);
    }
    else
    {
        node<T>* traverser = head;
        while (traverser != after_this)
        {
            traverser = traverser->_next;
        }
        node<T>* next_node = new node<T>;
        next_node->_item = insert_this;
        next_node->_prev = after_this;
        if (traverser->_next == nullptr)
        {
            traverser->_next = next_node;
            return next_node;
        }
        else
        {
            node<T>* original_next = traverser->_next;
            next_node->_next = original_next;
            original_next->_prev = next_node;
            traverser->_next = next_node;
            return next_node;
        }
    }
}

//insert before ptr: insert head if marker null
template <typename T> //DONE
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this)
{
    if (before_this == nullptr || head == before_this)
    {
        return _insert_head(head, insert_this);
    }
    else
    {
        node<T>* traverser = head;
        while (traverser->_next != before_this)
        {
            traverser = traverser->_next;
        }
        node<T>* insert_node = new node<T>;
        insert_node->_item = insert_this;
        insert_node->_next = before_this;
        insert_node->_prev = traverser;
        before_this->_prev = insert_node;
        traverser->_next = insert_node;
        return insert_node;
    }
}

//ptr to previous node
template <typename T>  //DONE
node<T>* _previous_node(node<T>* prev_to_this)
{
    if (prev_to_this == nullptr) { return nullptr; }
    return prev_to_this->_prev;
}

//delete, return item
template <typename T> //DONE
T _delete_node(node<T>*& head, node<T>* delete_this)
{
    assert(head != nullptr);
    if (head == delete_this)
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
        return old_head_item;
    }
    node<T>* traverser = head;
    while (traverser != nullptr && traverser != delete_this)
    {
        traverser = traverser->_next;
    }
    if (traverser == nullptr)
    {
        return T();
    }
    node<T>* next = traverser->_next;
    node<T>* prev = traverser->_prev;
    prev->_next = next;
    if (next != nullptr)
    {
        next->_prev = prev;
    }
    T deleted_item = traverser->_item;
    delete traverser;
    return deleted_item;
}

//duplicate the list...
template <typename T> //DONE
node<T>* _copy_list(node<T>* head)
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

//delete all the nodes
template <typename T> //DONE
void _clear_list(node<T>*& head)
{
    while (head != nullptr)
    {
        node<T>* old_head = head;
        head = head->_next;
        delete old_head;
    }
}

//_item at this position
template <typename T> //DONE
T& _at(node<T>* head, int pos)
{
    int curr_pos = 0;
    while (head != nullptr && curr_pos != pos)
    {
        head = head->_next;
    }
    if (curr_pos == pos)
    {
        return &(head->_item);
    }
    return nullptr;
}

#endif