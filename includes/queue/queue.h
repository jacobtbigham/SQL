#ifndef _QUEUE_H
#define _QUEUE_H

/**
 * @file queue.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief Single-ended queue (FIFO) structure
 * @version 0.1
 * @date 2021-06-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../node/node.h"
#include "../linked_list_functions/linked_list_functions.h"
#include <iostream>
#include <cassert>

using namespace std;

// Access to last-added (back) item is permitted only by value
// This is *not* a double-ended queue structure

// An Iterator with standard functionality is provided.

// FUNCTIONS IN THE PUBLIC INTERFACE:
// Queue()
// Queue(const Queue<T>& copy_me)
// ~Queue()
// Queue& operator=(const Queue<T>& RHS)
// bool empty()
// T front()
// T back()
// void push(T item)
// T pop()
// Iterator begin()
// Iterator end()
// int size()
// friend ostream& operator << (ostream& outs, const Queue<TT>& printMe)

template <typename T>
class Queue
{
public:
    /**
     * @brief Iterator class for Queue
     * 
     */
    class Iterator
    {
    public:
        friend class Queue;

        /**
         * @brief Construct a new Iterator object
         * 
         */
        Iterator()
        {
            _ptr = nullptr;
        }


        /**
         * @brief Construct a new Iterator object
         * 
         * @param p the node to initiate the Iterator to
         */
        Iterator(node<T>* p)
        {
            _ptr = p;
        }            


        /**
         * @brief Dereference operator for the Iterator
         * 
         * @return T the item at the current Iterator position
         */
        T operator *()
        {
            return _ptr->_item;
        }


        /**
         * @brief Return whether the the Iterator is null
         * 
         * @return true the Iterator references nullptr
         * @return false the Iterator references a valid node
         */
        bool is_null()
        {
            return _ptr == nullptr;
        }


        /**
         * @brief Inequality operator for Iterator
         * 
         * @param left the lefthand Iterator
         * @param right the righthand Iterator
         * @return true the two Iterators do not point to the same node
         * @return false the two Iterators point to the same node
         */
        friend bool operator !=(const Iterator& left, const Iterator& right)
        {
            return left._ptr != right._ptr;
        }


        /**
         * @brief Equality operator for Iterator
         * 
         * @param left the lefthand Iterator
         * @param right the righthand Iterator
         * @return true the two Iterators point to the same node
         * @return false the two Iterators do not point to the same node
         */
        friend bool operator ==(const Iterator& left, const Iterator& right)
        {
            return left._ptr == right._ptr;
        }


        /**
         * @brief Post-increment operator for Iterator
         * 
         * @return Iterator& the Iterator after incrementing (post)
         */
        Iterator& operator ++()
        {
            assert(_ptr != nullptr);
            _ptr = _ptr->_next;
            return *this;
        }


        /**
         * @brief Pre-increment operator for Iterator
         * 
         * @param it the Iterator to increment
         * @param _ unused
         * @return Iterator the Iterator as it was before incrementing
         */
        friend Iterator operator ++(Iterator& it, int _)
        {
            assert(it._ptr!=nullptr);
            Iterator old = it;
            it._ptr = it._ptr->_next;
            return old;
        }

    private:
        node<T>* _ptr; // pointer to the current node
    };


    /**
     * @brief Construct a new Queue object
     * 
     */
    Queue()
    {
        _front = nullptr;
        _rear = nullptr;
        _size = 0;
    }


    /**
     * @brief Construct a new Queue object by copy
     * 
     * @param copy_me the Queue to copy
     */
    Queue(const Queue<T>& copy_me)
    {
        if (copy_me._size == 0)
        {
            _front = nullptr;
            _rear = nullptr;
            _size = 0;
        }
        else if (copy_me._size == 1)
        {
            _size = 1;
            _front = _rear = new node<T>;
            _front->_item = copy_me._front->_item;
            _front->_next = _front->_prev = nullptr;
        }
        else
        {
            _size = 1;
            _front = new node<T>;
            _front->_item = copy_me._front->_item;
            node<T>* traverser = _front;
            Iterator it = copy_me.begin();
            it++;
            node<T>* next;
            while (it != copy_me.end())
            {
                next = new node<T>(*it);
                traverser->_next = next;
                next->_prev = traverser;
                traverser = next;
                _size++;
                it++;
            }
            _rear = next;
        }
    }


    /**
     * @brief Destroy the Queue object
     * 
     */
    ~Queue()
    {
        clear(_front);
    }


    /**
     * @brief Assignment operator for Queue
     * 
     * @param RHS the Queue to assign to the caller
     * @return Queue& the newly-assigned Queue
     */
    Queue& operator=(const Queue<T>& RHS)
    {
        if (this == &RHS) { return *this; }
        clear(_front);
        this = new Queue(RHS);
        return *this;
    }
    

    /**
     * @brief Return whether the Queue is empty
     * 
     * @return true the Queue has 0 elements
     * @return false the Queue has at least one element
     */
    bool empty()
    {
        return _size == 0;
    }


    /**
     * @brief Return the value at the front of the Queue
     * 
     * @return T the value at the front of the Queue
     */
    T front()
    {
        return _front->_item;
    }


    /**
     * @brief Return the value at the back of the Queue
     * 
     * @return T the value at the back of the Queue
     */
    T back()
    {
        return _rear->_item;
    }


    /**
     * @brief Add a new item to the back of the Queue
     * 
     * @param item the item to add to the back of the Queue
     */
    void push(T item)
    {
        node<T>* pushee = new node<T>(item);
        if (_front == nullptr)
        {
            _front = pushee;
            _rear = pushee;
            _front->_next = _front->_prev = nullptr;
        }
        else if (_front == _rear)
        {
            _rear = pushee;
            _front->_next = _rear;
            _rear->_prev = _front;
            _rear->_next = nullptr;
        }
        else
        {
            _rear->_next = pushee;
            pushee->_prev = _rear;
            _rear = pushee;
            _rear->_next = nullptr;
        }
        _size++;
    }


    /**
     * @brief Remove and return the value at the front of the Queue
     * 
     * @pre _front != nullptr
     * @return T the popped value
     */
    T pop()
    {
        assert(_front);
        T popped = _front->_item;
        if (_size == 1)
        {
            delete _front;
            _front = _rear = nullptr;
            _size = 0;
            return popped;
        }
        else if (_size == 2)
        {
            delete _front;
            _front = _rear;
            _rear->_prev = nullptr;
            _size = 1;
            return popped;
        }
        node<T>* new_front = _front->_next;
        delete _front;
        _front = new_front;
        _front->_prev = nullptr;
        _size--;
        return popped;
    }


    /**
     * @brief Return an Iterator to the front node
     * 
     * @return Iterator an Iterator to the front node
     */
    Iterator begin() const
    {
        return Iterator(_front);
    }


    /**
     * @brief Return an Iterator to nullptr
     * 
     * @return Iterator an Iterator to nullptr
     */
    Iterator end() const
    {
        return Iterator();
    }


    /**
     * @brief Return the size of the Queue
     * 
     * @return int the size of the Queue
     */
    int size() const
    {
        return _size;
    }


    /**
     * @brief Print the contents of the Queue to the specified output stream
     * 
     * @tparam TT the type of data stored in the Queue
     * @param outs the output stream to print to 
     * @param print_me the Queue to print
     * @return ostream& the output stream printed to
     */
    template<typename TT>
    friend ostream& operator << (ostream& outs, const Queue<TT>& print_me)
    {
        Iterator it = print_me.begin();
        while (it != print_me.end())
        {
            cout << "[" << *it << "]->";
            it++;
        }
        cout << "|||";
        return outs;
    }

private:
    node<T>* _front; // first-in
    node<T>* _rear; // last-in
    int _size;
};

#endif