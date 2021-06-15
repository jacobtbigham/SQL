#ifndef MULTIMAP_H
#define MULTIMAP_H

#include <string>
#include <cstring>
#include "./bplustree.h"
#include <iomanip>
#include <iostream>
using namespace std;

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K())
    {
        key = k;
        // do I need anything here?
    }

    MPair(const K& k, const V& v)
    {
        key = k;
        value_list.push_back(v);
    }
    MPair(const K& k, const vector<V>& vlist)
    {
        key = k;
        for (V x : vlist)
        {
            value_list.push_back(x);
        }
    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me)
    {
        if (print_me.value_list.size() == 0)
        {
            outs << "empty iterator";
            return outs;
        }
        outs << "<" << print_me.key << ", [";
        for (int i = 0, size = print_me.value_list.size(); i < size - 1; ++i)
        {
            outs << to_string(print_me.value_list[i]) << ", ";
        }
        outs << to_string(print_me.value_list.back()) << "]>";
        return outs;
    }
    
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key == rhs.key;
    }

    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key < rhs.key;
    }

    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key <= rhs.key;
    }

    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key > rhs.key;
    }

    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        // what is this supposed to do?
        MPair<K, V> sum = MPair<K, V>();
        sum.key = lhs.key + rhs.key;
        return sum;
    }
};


template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it)
        {
            _it = it;
        }

        Iterator operator ++(int unused) // post
        {
            return _it++;
        }

        Iterator operator ++()
        {
            return ++_it;
        }

        MPair<K, V> operator *()
        {
            return *(_it);
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            return lhs._it == rhs._it;
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap()
    {
        mmap = new BPlusTree< MPair<K, V> >();
    }

//  Iterators
    Iterator begin()
    {
        return mmap.begin();
    }

    Iterator end()
    {
        return mmap.end();
    }

//  Capacity
    int size() const
    {
        return mmap.size();
    }
    
    bool empty() const
    {
        return mmap.empty();
    }

//  Element Access
    const vector<V>& operator[](const K& key) const
    {
        MPair<K, V> find_item = MPair<K, V>(key);
        return mmap.get(find_item).value_list;
    }

    vector<V>& operator[](const K& key)
    {
        MPair<K, V> find_item = MPair<K, V>(key);
        return mmap.get(find_item).value_list;
    }

//  Modifiers
    void insert(const K& k, const V& v)
    {
        if (contains(k))
        {
            MPair<K, V> dummy = MPair<K, V>(k);
            mmap.get(dummy).value_list.push_back(v);
        }
        else
        {
            MPair<K, V> new_item = MPair<K, V>(k, v);
            mmap.insert(new_item);
        }
    }

    void erase(const K& key)
    {
        MPair<K, V> erase_item = MPair<K, V>(key, V());
        mmap.remove(erase_item);
    }

    void clear()
    {
        mmap.clear_tree();
    }

//  Operations:
    bool contains(const K& key) const
    {
        MPair<K, V> dummy = MPair<K, V>(key);
        return mmap.contains(dummy);
    }

    vector<V>& get(const K& key)
    {
        MPair<K, V> find_item = MPair<K, V>(key);
        return mmap.get(find_item).value_list;
    }

    Iterator find(const K& key)
    {
        MPair<K, V> find_item = MPair<K, V>(key);
        return mmap.find(find_item);
    }

    Iterator upper_bound(const K& key)
    {
        return mmap.upper_bound(key);
    }

    Iterator lower_bound(const K& key)
    {
        return mmap.lower_bound(key);
    }

    int count(const K& key)
    {
        if (! contains(key)) { return 0; }
        vector<V> vec = get(key);
        return vec.size();
        
    }

    // I have not writtent hese yet, but we will need them:
    //    lower_bound
    //    upper_bound
    //    equal_range:



    bool is_valid()
    {
        return mmap.is_valid();
    }

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

    void print_helper(K& first, vector<V>& second)
    {
        cout << setw(10) << first << " :    " << second << endl;
    }

    void print_lookup()
    {
        Iterator start = begin();
        Iterator finish = end();
        while (start != finish)
        {
            MPair<K, V> curr = *start;
            print_helper(curr.key, curr.value_list);
            ++start;
        }
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};



#endif