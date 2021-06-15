#ifndef MAP_H
#define MAP_H

#include <string>
#include "./bplustree.h"
#include <iostream>
using namespace std;

template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V()) { key = k; value = v;}

    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me)
    {
        outs << "<" << to_string(print_me.key) << ", " << to_string(print_me.value) << ">";
        return outs;
    }

    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return lhs.key == rhs.key;
    }

    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return lhs.key < rhs.key;
    }

    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return lhs.key > rhs.key;
    }

    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return lhs.key <= rhs.key;
    }

    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        // what is this supposed to do?
        Pair<K, V> sum = Pair();
        sum.key = lhs.key + rhs.key;
        return sum;
    }
};

template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K, V> > map_base;

    class Iterator{
    public:
        friend class Map;

        Iterator(typename map_base::Iterator it)
        {
            _it = it;
        }

        Iterator operator ++(int unused) //post
        {
            _it++;
        }

        Iterator operator ++()
        {
            ++_it;
        }

        Pair<K, V> operator *()
        {
            return *(_it);
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            return lhs._it == rhs._it;
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            return lhs._it != rhs._it;
        }

    private:
        typename map_base::Iterator _it;
    };

    Map()
    {
        map = new BPlusTree< Pair<K, V> >();
        key_count = 0;
    }

//  Iterators
    Iterator begin()
    {
        return map.begin();
    }

    Iterator end()
    {
        return map.end();
    }

//  Capacity
    int size() const
    {
        return key_count;
    }
    bool empty() const
    {
        return key_count == 0;
    }

//  Element Access
    V& operator[](const K& key)
    {
        return at(key);
    }

    V& at(const K& key)
    {
        Pair<K, V> find_item = Pair<K, V>(key);
        return  map.get(find_item).value;
    }

    const V& at(const K& key) const
    {
        Pair<K, V> find_item = Pair<K, V>(key);
        return map.get(find_item).value;
    }


//  Modifiers
    void insert(const K& k, const V& v)
    {
        Pair<K, V> new_item = Pair<K, V>(k, v); // does this need to be new?
        map.insert(new_item);
        key_count = int(map.size());
    }

    void erase(const K& key)
    {
        Pair<K, V> erase_item = Pair<K, V>(key, V());
        map.remove(erase_item);
        key_count = int(map.size());
    }

    void clear()
    {
        map.clear_tree();
        key_count = 0;
    }

    V get(const K& key)
    {
        Pair<K, V> find_item = Pair<K, V>(key, V());
        V return_item = map.get(find_item).value;
        return return_item;
    }

//  Operations:
    Iterator find(const K& key)
    {
        Pair<K, V> find_item = Pair<K, V>(key, V());
        return map.find(find_item);
    }

    Iterator upper_bound(const K& key)
    {
        return map.upper_bound(key);
    }

    Iterator lower_bound(const K& key)
    {
        return map.lower_bound(key);
    }

    bool contains(const Pair<K, V>& target) const
    {
        return map.contains(target);
    }

    bool is_valid(){ return map.is_valid(); }

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map<<endl;
        return outs;
    }

private:
    int key_count;
    BPlusTree< Pair<K, V> > map;
};






#endif