#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include <string>
#include <iostream>
#include <stack>
#include "./btree_array_funcs.h"
using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;

        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0)
        {
            it = _it;
            key_ptr = _key_ptr;
        }

        T operator *()
        {
            if (it) { return it->data[key_ptr]; }
            else { return T(); } // ?????
        }

        Iterator operator++(int un_used) //post
        {
            Iterator before = Iterator(it, key_ptr);
            ++(*this);
            return before;
        }

        Iterator operator++()
        {
            if (it == nullptr) { return *this; }
            else
            {
                ++key_ptr;
                if (key_ptr >= it->data_count)
                {
                    key_ptr = 0;
                    it = it->next;
                }
                return *this;
            }
        }


        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            return (lhs.it == rhs.it) && (lhs.key_ptr == rhs.key_ptr);
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            return (lhs.it != rhs.it) || (lhs.key_ptr != rhs.key_ptr);
        }

        void print_Iterator(){
            if (it){
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }

        bool is_null() { return ! it; }

        void info()
        {
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false)
    {
        dups_ok = dups;
        data_count = 0;
        child_count = 0;
        next = nullptr;
    }

    BPlusTree(T *a, int size, bool dups = false)
    {
        dups_ok = dups;
        data_count = 0;
        child_count = 0;
        next = nullptr;
        for (int i = 0; i < size; ++i)
        {
            insert(a[i]);
        }
    }

    //big three:
    BPlusTree(const BPlusTree<T>& other)
    {
        dups_ok = other.dups_ok;
        data_count = 0;
        child_count = 0;
        next = nullptr;
        copy_tree(other);
    }

    ~BPlusTree() { clear_tree(); }

    //right now, this copies and doesn't assign
    BPlusTree<T>& operator=(const BPlusTree<T>& RHS)
    {
        if (this != &RHS)
        {
            clear_tree();
            copy_tree(RHS);
        }
        return *this;
    }

    void copy_tree(const BPlusTree<T>& other)  //copy other into this object
    {
        if (this->data_count != 0 && this != &other) { clear_tree(); }
        copy_array(data, other.data, data_count, other.data_count);
        if (other.is_leaf())
        {
            child_count = 0;
        }
        else
        {
            child_count = other.child_count;
            BPlusTree<T>* dummy = nullptr;
            for (int i = 0; i < other.child_count; ++i)
            {
                subset[i] = copy_helper(other.subset[i], &dummy);
            }
        }
    }

    BPlusTree<T>* copy_helper(const BPlusTree<T>* other, BPlusTree<T>* *last_leaf)
    {
        BPlusTree<T>* new_tree = new BPlusTree();
        copy_array(new_tree->data, other->data, new_tree->data_count, other->data_count);
        if (other->is_leaf())
        {
            new_tree->child_count = 0;
            if (*last_leaf == nullptr) { *last_leaf = new_tree; }
            else
            {
                (*last_leaf)->next = new_tree;
                *last_leaf = new_tree;
            }
            return new_tree;
        }
        new_tree->child_count = other->child_count;
        for (int i = 0; i < other->child_count; ++i)
        {
            new_tree->subset[i] = copy_helper(other->subset[i], last_leaf);
        }
        return new_tree;
    }

    void insert(const T& entry)   //insert entry into the tree
    {
        loose_insert(entry);
        if (data_count > MAXIMUM)
        {
            BPlusTree* new_tree = new BPlusTree();
            copy_array(new_tree->data, data, new_tree->data_count, data_count);
            copy_array(new_tree->subset, subset, new_tree->child_count, child_count);
            data_count = 0;
            child_count = 1;
            subset[0] = new_tree;
            fix_excess(0);
        }
    }

    void remove(const T& entry)   //remove entry from the tree
    {
        if (empty()) { return; }
        loose_remove(entry);
        if (data_count == 0 && child_count == 1)
        {
            BPlusTree* child = subset[0];
            copy_array(data, child->data, data_count, child->data_count);
            copy_array(subset, child->subset, child_count, child->child_count);
        }
        fix_breadcrumb(entry);
    }


    void clear_tree()             //clear this object (delete all nodes etc.)
    {
        if (is_leaf())
        {
            data_count = 0;
            next = nullptr;
        }
        else
        {
            data_count = 0;
            for (int i = 0; i < child_count; ++i)
            {
                subset[i]->clear_tree();
            }
            child_count = 0;
            next = nullptr;
        }
    }

    bool contains(const T& entry) const //true if entry can be found
    {
        int i = first_ge(data, data_count, entry);
        if (i == data_count && is_leaf()) { return false; }
        else if ((i < data_count) && !(entry < data[i]) && is_leaf()) { return true; }
        else if ((i < data_count) && !(entry < data[i]))
        {
            return subset[i+1]->contains(entry);
        }
        else if (! is_leaf()) { return subset[i]->contains(entry); }
        else { return false; }
    }

    T& get(const T& entry)              //return a reference to entry
    {
        return *find_ptr(entry);
    }

    const T& get(const T& entry) const   //return a reference to entry
    {
        return *find_ptr(entry);
    }

    T& get_existing(const T& entry)     //return a reference to entry
    {
        return *find_ptr(entry);
    }

    Iterator find(const T& key)         //return an iterator to this key, NULL if not there.
    {
        bool found = contains(key);
        if (found)
        {
            int i = first_ge(data, data_count, key);
            BPlusTree* ptr = this;
            while (! (ptr->is_leaf()))
            {
                ptr = ptr->subset[i];
            }
            i = first_ge(ptr->data, ptr->data_count, key);
            return Iterator(ptr, i); // do I need new here?
        }
        else { return NULL; }
    }

    friend ostream& operator<<(ostream& outs, const Iterator& print_me)
    {
        if (print_me.it != nullptr)
        {
            outs << print_me.it->data[print_me.key_ptr];
        }
        return outs;
    }

    Iterator lower_bound(const T& key)  //return first that goes NOT BEFORE key entry or next if does not exist: >= entry
    {
        BPlusTree<T>* ptr = get_smallest_node();
        while (ptr != nullptr)
        {
            for (int i = 0; i < ptr->data_count; ++i)
            {
                if (!(ptr->data[i] < key))
                {
                    return Iterator(ptr, i);
                }
            }
            ptr = ptr->next;
        }
        return Iterator(ptr);
    }

    Iterator upper_bound(const T& key)  //return first that goes AFTER key exist or not, the next entry  >entry
    {
        BPlusTree<T>* ptr = get_smallest_node();
        while (ptr != nullptr)
        {
            for (int i = 0; i < ptr->data_count; ++i)
            {
                if (ptr->data[i] > key)
                {
                    return Iterator(ptr, i);
                }
            }
            ptr = ptr->next;
        }
        return Iterator(ptr);
    }


    int size() const                    //count the number of elements
    {
        if (is_leaf()) { return data_count; }
        int count = 0;
        for (int i = 0; i < child_count; ++i)
        {
            count += subset[i]->size();
        }
        return count;
    }

    bool empty() const                  //true if the tree is empty
    {
        return (data_count == 0) && (child_count == 0);
    }

    void print_tree(int level = 0, ostream &outs=cout) const
    {
        if (empty()) { return; }
        int padding = 4;
        if (! is_leaf())
        {
            subset[child_count-1]->print_tree(level+1);
            for (int i = child_count - 2; i >= 0; --i)
            {
                for (int j = 0; j < padding * level; ++j) { outs << " "; }
                outs << data[i]  << endl;
                subset[i]->print_tree(level+1);
            }
        }
        else if (data_count > 0)
        {
            for (int j = 0; j < padding * level; ++j) { outs << " "; }
            print_array(data, data_count);
            outs << endl;
        }
    }

    friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me)
    {
        print_me.print_tree(0, outs);
        return outs;
    }
    

    bool is_valid()
    {
        return true;
    }

    std::string in_order()
    {
        if (empty()) { return ""; }
        if (data_count == 1 && child_count == 0) { return to_string(data[0]) + " |";}
        std::string result = "";
        BPlusTree<T>* smallest = get_smallest_node();
        while (smallest != nullptr)
        {
            for (int i = 0, count = smallest->data_count; i < count; ++i)
            {
                result += to_string(smallest->data[i]) + " | ";
            }
            smallest = smallest->next;
        }
        return result;
    }

    Iterator begin()
    {
        BPlusTree* smallest = get_smallest_node();
        Iterator result = Iterator(smallest, 0);
        return result;
    }

    Iterator end()
    {
        return Iterator();
    }

    ostream& list_keys(Iterator from = NULL, Iterator to = NULL)
    {
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;
    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next;

    bool is_leaf() const { return child_count == 0; }             //true if this is a leaf node

    T* find_ptr(const T& entry)         //return a pointer to this key. NULL if not there.
    {
        int i = first_ge(data, data_count, entry);
        if (is_leaf() && (i < data_count) && !(entry < data[i]))
        {
            return &(data[i]);
        }
        else if ((i < data_count) && !(entry < data[i]))
        {
            return subset[i+1]->find_ptr(entry);
        }
        else if (! is_leaf()) { return subset[i]->find_ptr(entry); }
        else { return NULL; }
    }

    void loose_insert(const T& entry)   //allows MAXIMUM+1 data elements in the root
    {
        int i = first_ge(data, data_count, entry);
        bool found = contains(entry);
        if (is_leaf() && found)
        {
            if (dups_ok)
            {
                insert_item(data, i, data_count, entry);
            }
            else
            {
                data[i] = entry;
            }
        } //duplicate
        else if (is_leaf() && ! found)
        {
            insert_item(data, i, data_count, entry);
        }
        // else if (! is_leaf() && found)
        // {
        //     subset[i+1]->loose_insert(entry);
        //     fix_excess(i+1);
        // }
        else
        {
            subset[i]->loose_insert(entry);
            fix_excess(i);
        }
    }


    void fix_excess(int i)              //fix excess in child i
    {
        if (subset[i]->data_count <= MAXIMUM) { return; }
        else
        {
            T middle;
            bool leaf = subset[i]->is_leaf();
            delete_item(subset[i]->data, (subset[i]->data_count) / 2, subset[i]->data_count, middle);
            insert_item(data, i, data_count, middle);
            BPlusTree* new_tree = new BPlusTree();
            split(subset[i]->data, subset[i]->data_count, new_tree->data, new_tree->data_count);
            split(subset[i]->subset, subset[i]->child_count, new_tree->subset, new_tree->child_count);
            insert_item(subset, i+1, child_count, new_tree);
            if (leaf)
            {
                new_tree->next = subset[i]->next;
                insert_item(subset[i+1]->data, 0, subset[i+1]->data_count, middle);
                subset[i]->next = subset[i+1];
                if (i + 2 < child_count)
                {
                    subset[i+1]->next = subset[i+2];
                }
            }
        }
    }


    void loose_remove(const T& entry)  //allows MINIMUM-1 data elements in the root
    {
        int i = first_ge(data, data_count, entry);
        if (is_leaf() && i == data_count) { return; } // leaf and not present
        else if (is_leaf() && !(entry < data[i])) // leaf and present
        {
            T removed;
            delete_item(data, i, data_count, removed);
        }
        else if ((i != data_count) && !(entry < data[i]))
        {
            subset[i+1]->loose_remove(entry);
            if (subset[i+1]->data_count < MINIMUM)
            {
                fix_shortage(i+1);
            }
        }
        else
        {
            subset[i]->loose_remove(entry);
            if (subset[i]->data_count < MINIMUM)
            {
                fix_shortage(i);
            }
        }
    }

    // not sure whether this works??
    void fix_breadcrumb(const T& key)
    {
        BPlusTree* ptr = this;
        bool done = false;
        while (! done)
        {
            int i = first_ge(ptr->data, ptr->data_count, key);
            if (i < ptr->data_count && !(key < ptr->data[i]))
            {
                ptr->data[i] = ptr->subset[i+1]->get_smallest_node()->data[0];
                done = true;
            }
            else
            {
                if (ptr->is_leaf()) { done = true; }
                else { ptr = ptr->subset[i]; }
            }
        }
    }

    // need to account for inner fix shortages as well (if/else for leaves, not just always doing the leaf procedure)
    BPlusTree<T>* fix_shortage(int i)  //fix shortage in child i
    {
        if (is_leaf() || subset[i]->data_count > MINIMUM ) { return nullptr; }
        
        if (i > 0 && subset[i-1]->data_count > MINIMUM) // case 1
        {
            if (subset[i-1]->is_leaf())
            {
                detach_item(subset[i-1]->data, subset[i-1]->data_count, subset[i]->data[0]);
                (subset[i]->data_count)++;
                data[i-1] = subset[i]->data[0];
            }
            else
            {
                insert_item(subset[i]->data, 0, subset[i]->data_count, data[i-1]);
                delete_item(subset[i-1]->data, subset[i-1]->data_count - 1, subset[i-1]->data_count, data[i-1]);
                insert_item(subset[i]->subset, 0, subset[i]->child_count, subset[i-1]->subset[--subset[i-1]->child_count]);
            }
        }
        else if (i < child_count - 1 && subset[i+1]->data_count > MINIMUM) // case 2
        {
            if (subset[i+1]->is_leaf())
            {
                delete_item(subset[i+1]->data, 0, subset[i+1]->data_count, subset[i]->data[0]);
                (subset[i]->data_count)++;
                data[i] = subset[i+1]->data[0];
                return this;
            }
            else
            {
                insert_item(subset[i]->data, subset[i]->data_count, subset[i]->data_count, data[i]);
                delete_item(subset[i+1]->data, 0, subset[i+1]->data_count, data[i]);
                insert_item(subset[i]->subset, subset[i]->child_count, subset[i]->child_count, subset[i+1]->subset[0]);
                BPlusTree* dummy;
                delete_item(subset[i+1]->subset, 0, subset[i+1]->child_count, dummy);
            }
        }
        else if (i > 0 && subset[i-1]->data_count == MINIMUM) // case 3
        {
            if (subset[i]->is_leaf())
            {
                T dummy;
                delete_item(data, i-1, data_count, dummy);
                BPlusTree* old_next = subset[i]->next;
                merge(subset[i-1]->data, subset[i-1]->data_count, subset[i]->data, subset[i]->data_count);
                merge(subset[i-1]->subset, subset[i-1]->child_count, subset[i]->subset, subset[i]->child_count);
                BPlusTree* dummy_2;
                delete_item(subset, i, child_count, dummy_2);
                subset[i-1]->next = old_next;
            }
            else
            {
                insert_item(subset[i-1]->data, subset[i-1]->data_count, subset[i-1]->data_count, data[i-1]);
                T dummy;
                delete_item(data, i-1, data_count, dummy);
                merge(subset[i-1]->data, subset[i-1]->data_count, subset[i]->data, subset[i]->data_count);
                merge(subset[i-1]->subset, subset[i-1]->child_count, subset[i]->subset, subset[i]->child_count);
                BPlusTree* dummy_2;
                delete_item(subset, i, child_count, dummy_2);
            }
            
        }
        else // case 4
        {
            if (subset[i]->is_leaf())
            {
                T dummy;
                delete_item(data, i, data_count, dummy);
                BPlusTree* old_next = subset[i+1]->next;
                merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
                merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);
                BPlusTree* dummy_2;
                delete_item(subset, i+1, child_count, dummy_2);
                subset[i]->next = old_next;
            }
            else
            {
                insert_item(subset[i]->data, subset[i]->data_count, subset[i]->data_count, data[i]);
                T dummy;
                delete_item(data, i, data_count, dummy);
                merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
                merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);
                BPlusTree* dummy_2;
                delete_item(subset, i+1, child_count, dummy_2);
            }
        }
        return this;
    }

    
    BPlusTree<T>* get_smallest_node() // and return the smallest key in this subtree
    {
        if (empty()) { return nullptr; }
        else if (is_leaf()) { return this; }
        else { return subset[0]->get_smallest_node(); }
    }

    BPlusTree<T>* get_largest_node() // and return the smallest key in this subtree
    {
        if (empty()) { return nullptr; }
        else if (is_leaf()) { return this; }
        else { return subset[child_count-1]->get_largest_node(); }
    }

    void get_smallest(T& entry)      //entry := leftmost leaf
    {
        if (is_leaf())
        {
            entry = data[0];
        }
        else
        {
            subset[0]->get_smallest(entry);
        }
    }

    void get_biggest(T& entry)       //entry := rightmost leaf
    {
        if (is_leaf())
        {
            entry = data[data_count - 1];
        }
        else
        {
            subset[child_count - 1]->get_smallest(entry);
        }
    }

    void remove_biggest(T& entry)    //remove the biggest child of tree->entry
    {
        if (is_leaf())
        {
            delete_item(data, data_count - 1, data_count, entry);
        }
        else
        {
            subset[child_count - 1]->remove_biggest(entry);
            if (subset[child_count - 1]->data_count < MINIMUM)
            {
                fix_shortage(child_count - 1);
            }
        }
    }

    void transfer_left(int i);        //transfer one element LEFT from child i

    void transfer_right(int i);       //transfer one element RIGHT from child i

    BPlusTree<T> *merge_with_next_subset(int i); //merge subset i with  i+1

};


#endif