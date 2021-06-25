#ifndef _STACK_H
#define _STACK_H

/**
 * @file stack.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief Single-ended stack (LIFO) structure
 * @version 0.1
 * @date 2021-06-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../node/node.h"
#include "../linked_list_functions/linked_list_functions.h"
#include <iostream>
#include <cassert>

using namespace std;

// An Iterator with standard functionality is provided.

// FUNCTIONS IN THE PUBLIC INTERFACE:
// Stack()
// Stack(const Stack<T>& copy_me)
// ~Stack()
// Stack<T>& operator=(const Stack<T>& RHS)
// bool empty()
// T top()
// void push(T item)
// T pop()
// Iterator begin()
// Iterator end()
// int size()
// friend ostream& operator << (ostream& outs, const Stack<TT>& print_me)

template <typename T>
class Stack{
public:
    /**
     * @brief Iterator class for Stack
     *  
     */
    class Iterator{
    public:
        friend class Stack;
        
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
     * @brief Construct a new empty Stack object
     * 
     */
    Stack()
    {
        _top = nullptr;
        _size = 0;
    }


    /**
     * @brief Construct a new Stack object by copy
     * 
     * @param copy_me the Stack to copy
     */
    Stack(const Stack<T>& copy_me)
    {
        _top = duplicate(copy_me._top);
        _size = copy_me._size;
    }
    

    /**
     * @brief Destroy the Stack object
     * 
     */
    ~Stack()
    {
        clear(_top);
    }


    /**
     * @brief Assignment operator for Stack
     * 
     * @param RHS the Stack to assign to the caller
     * @return Stack<T>& the newly-assigned Stack
     */
    Stack<T>& operator=(const Stack<T>& RHS)
    {
        if (this == &RHS) { return *this; }
        clear(_top);
        _top = duplicate(RHS._top);
        _size = RHS._size;
    }


    /**
     * @brief Return whether the Stack is empty
     * 
     * @return true the Stack has 0 elements
     * @return false the Stack has at least one element
     */
    bool empty()
    {
        return _size == 0;
    }


    /**
     * @brief Return the value at the top of the Stack
     * 
     * @return T the value at the top of the Stack
     */
    T top()
    {
        assert(_top);
        return _top->_item;
    }


    /**
     * @brief Add a new item to the top of the Stack
     * 
     * @param item the item to att to the top of the Stack
     */
    void push(T item)
    {
        node<T>* pushee = new node<T>(item);
        if (_top == nullptr)
        {
            _top = pushee;
        }
        else
        {
            _top->_prev = pushee;
            pushee->_next = _top;
            _top = pushee;
        }
        _size++;
    }


    /**
     * @brief Remove and return the value at the top of the Stack
     * 
     * @pre _top != nullptr
     * @return T the popped value
     */
    T pop()
    {
        assert(_top);
        T popped = _top->_item;
        if (_size == 1)
        {
            delete _top;
            _top = nullptr;
            _size = 0;
            return popped;
        }
        else
        {
            node<T>* new_top = _top->_next;
            delete _top;
            _top = new_top;
            _top->_prev = nullptr;
            _size--;
            return popped;
        }
    }


    /**
     * @brief Return an Iterator to the top node
     * 
     * @return Iterator an Iterator to the top node
     */
    Iterator begin() const
    {
        return Iterator(_top);
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
     * @brief Return the size of the Stack
     * 
     * @return int the size of the Stack
     */
    int size() const
    {
        return _size;
    }


    /**
     * @brief Print the contents of the Stack to the specified output stream
     * 
     * @tparam TT the type of data stored in the Stack
     * @param outs the output stream to print to
     * @param print_me the Stack to print
     * @return ostream& the output stream printed to
     */
    template<typename TT>
    friend ostream& operator<<(ostream& outs, const Stack<TT>& print_me)
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
    node<T>* _top; // last-in
    int _size;
};

#endif