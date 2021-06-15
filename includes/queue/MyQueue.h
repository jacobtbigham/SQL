#ifndef _QUEUE_H
#define _QUEUE_H

#include "../node/node.h"
#include "../linked_list_functions/linked_list_functions.h"
#include <iostream>
#include <cassert>
using namespace std;

template <typename T>
class Queue
{
public:
    class Iterator{
    public:
        friend class Queue;                               //give access to list to access _ptr
        Iterator(){_ptr = nullptr;}                          //default ctor
        Iterator(node<T>* p) //Point Iterator to where p is pointing to
        {
            _ptr = p;
        }            

        T operator *()
        {
            return _ptr->_item;
        }

        bool is_null() //true if _ptr is NULL
        {
            return _ptr == nullptr;
        }            
        friend bool operator !=(const Iterator& left,
                                const Iterator& right)  //true if left != right
        {
            return left._ptr != right._ptr;
        }

        friend bool operator ==(const Iterator& left,
                                const Iterator& right)  //true if left == right
        {
            return left._ptr == right._ptr;
        }


        Iterator& operator++(){                         //member operator:++it
                                                        // or ++it = new_value
            assert(_ptr!=nullptr);
            _ptr = _ptr->_next;
            return *this;
        }

        friend Iterator operator++(Iterator& it,
                                   int unused){         //friend operator: it++
            assert(it._ptr!=nullptr);
            Iterator old = it;
            it._ptr = it._ptr->_next;
            return old;
        }

    private:
        node<T>* _ptr;                          //pointer being encapsulated
    };

    Queue()
    {
        _front = nullptr;
        _rear = nullptr;
        _size = 0;
    }

    Queue(const Queue<T>& copyMe)
    {
        if (copyMe._size == 0)
        {
            _front = nullptr;
            _rear = nullptr;
            _size = 0;
        }
        else if (copyMe._size == 1)
        {
            _size = 1;
            _front = _rear = new node<T>;
            _front->_item = copyMe._front->_item;
            _front->_next = _front->_prev = nullptr;
        }
        else
        {
            _front = new node<T>;
            _front->_item = copyMe._front->_item;
            node<T>* traverser = _front;
            _size = 1;
            Iterator it = copyMe.begin();
            it++;
            node<T>* next;
            while (it != copyMe.end())
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

    ~Queue()
    {
        _clear_list(_front);
    }

    Queue& operator=(const Queue<T>& RHS)
    {
        if (this == &RHS) { return *this; }
        _clear_list(_front);
        if (RHS._size == 0)
        {
            _front = nullptr;
            _rear = nullptr;
            _size = 0;
        }
        else if (RHS._size == 1)
        {
            _size = 1;
            _front = _rear = new node<T>;
            _front->_item = RHS._front->_item;
            _front->_next = _front->_prev = nullptr;
        }
        else
        {
            _front = new node<T>;
            _front->_item = RHS._front->_item;
            node<T>* traverser = _front;
            _size = 1;
            Iterator it = RHS.begin();
            it++;
            node<T>* next;
            while (it != RHS.end())
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
        return *this;
    }

    bool empty()
    {
        return _size == 0;
    }

    T front()
    {
        return _front->_item;
    }

    T back()
    {
        return _rear->_item;
    }

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

    T pop() // what happens when size is 2?
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

    Iterator begin() const //Iterator to the head node
    {
        return Iterator(_front);
    }                                     
    Iterator end() const //Iterator to NULL
    {
        return Iterator();
    }

    void print_pointers()
    {
        cout << "Front: " << _front << endl;
        cout << "Read:  " << _rear << endl;
    }

    int size() const { return _size; }

    // template<typename TT>
    // friend ostream& operator << (ostream& outs, const Queue<TT>& printMe)
    // {
    //     Iterator it = printMe.begin();
    //     while (it != printMe.end())
    //     {
    //         cout << "[" << *it << "]->";
    //         it++;
    //     }
    //     cout << "|||";
    //     return outs;
    // }
private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};

#endif