#ifndef _STACK_H
#define _STACK_H

#include "../node/node.h"
#include "../linked_list_functions/linkedlistfunctionsTemplated.h"
#include <iostream>
#include <cassert>
using namespace std;

template <typename ITEM_TYPE>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                 //give access to list to access _ptr
        Iterator(){ _ptr = nullptr; }            //default ctor
        Iterator(node<ITEM_TYPE>* p) //Point Iterator to where p is pointing to
        {
            _ptr = p;
        }      
                                            
        ITEM_TYPE operator *() //dereference operator
        {
            return _ptr->_item; 
        }    

        bool is_null() //true if _ptr is NULL
        {
            return _ptr == NULL;
        }            

        friend bool operator !=(const Iterator& left, const Iterator& right)  //true if left != right
        {
            return left._ptr != right._ptr;
        }

        friend bool operator ==(const Iterator& left, const Iterator& right)  //true if left == right
        {
            return left._ptr == right._ptr;
        }


        Iterator& operator++() //member operator:++it or ++it = new_value
        {   assert(_ptr!=nullptr);                  
            _ptr = _ptr->_next;
            return *this;  
        }

        friend Iterator operator++(Iterator& it,
                                   int unused){        //friend operator: it++
            assert(it._ptr!=nullptr);
            Iterator old = it;
            it._ptr = it._ptr->_next;
            return old;
        }

    private:
        node<ITEM_TYPE>* _ptr;    //pointer being encapsulated
    };

    Stack()
    {
        _top = nullptr;
        _size = 0;
    }

    Stack(const Stack<ITEM_TYPE>& copyMe)
    {
        _top = _copy_list(copyMe._top);
        _size = copyMe._size;
    }
    
    ~Stack()
    {
        _clear_list(_top);
    }

    Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS)
    {
        if (this == &RHS) { return *this; }
        _clear_list(_top);
        _top = _copy_list(RHS._top);
        _size = RHS._size;
    }

    ITEM_TYPE top()
    {
        assert(_top);
        return _top->_item;
    }

    bool empty()
    {
        return _size == 0;
    }

    void push(ITEM_TYPE item)
    {
        node<ITEM_TYPE>* pushee = new node<ITEM_TYPE>(item);
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

    ITEM_TYPE pop()
    {
        assert(_top);
        ITEM_TYPE popped = _top->_item;
        if (_size == 1)
        {
            delete _top;
            _top = nullptr;
            _size = 0;
            return popped;
        }
        else
        {
            node<ITEM_TYPE>* new_top = _top->_next;
            delete _top;
            _top = new_top;
            _top->_prev = nullptr;
            _size--;
            return popped;
        }
    }

    // template<typename T>
    // friend ostream& operator<<(ostream& outs, const Stack<T>& printMe)
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

    Iterator begin() const //Iterator to the head node
    {
        return Iterator(_top);
    }                   

    Iterator end() const //Iterator to NULL
    {
        return Iterator();
    }     

    int size() const { return _size; }

private:
    node<ITEM_TYPE>* _top;
    int _size;
};

#endif