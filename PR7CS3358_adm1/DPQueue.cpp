// FILE: DPQueue.cpp
// IMPLEMENTS: p_queue (see DPQueue.h for documentation.)
//
// INVARIANT for the p_queue class:
//   1. The number of items in the p_queue is stored in the member
//      variable used.
//   2. The items themselves are stored in a dynamic array (partially
//      filled in general) organized to follow the usual heap storage
//      rules.
//      2.1 The member variable heap stores the starting address
//          of the array (i.e., heap is the array's name). Thus,
//          the items in the p_queue are stored in the elements
//          heap[0] through heap[used - 1].
//      2.2 The member variable capacity stores the current size of
//          the dynamic array (i.e., capacity is the maximum number
//          of items the array currently can accommodate).
//          NOTE: The size of the dynamic array (thus capacity) can
//                be resized up or down where needed or appropriate
//                by calling resize(...).
// NOTE: Private helper functions are implemented at the bottom of
// this file along with their precondition/postcondition contracts.

#include <cassert>   // provides assert function
#include <iostream>  // provides cin, cout
#include <iomanip>   // provides setw
#include <cmath>     // provides log2
#include <string.h>  // provides memcpy
#include "DPQueue.h"

using namespace std;

namespace CS3358_FA17A7
{
    // EXTRA MEMBER FUNCTIONS FOR DEBUG PRINTING
    void p_queue::print_tree(const char message[], size_type i) const
    // Pre:  (none)
    // Post: If the message is non-empty, it has first been written to
    //       cout. After that, the portion of the heap with root at
    //       node i has been written to the screen. Each node's data
    //       is indented 4*d, where d is the depth of the node.
    //       NOTE: The default argument for message is the empty string,
    //             and the default argument for i is zero. For example,
    //             to print the entire tree of a p_queue p, with a
    //             message of "The tree:", you can call:
    //                p.print_tree("The tree:");
    //             This call uses the default argument i=0, which prints
    //             the whole tree.
    {
        const char NO_MESSAGE[] = "";
        size_type depth;
        
        if (message[0] != '\0')
            cout << message << endl;
        
        if (i >= used)
            cout << "(EMPTY)" << endl;
        else
        {
            depth = size_type( log( double(i+1) ) / log(2.0) + 0.1 );
            if (2*i + 2 < used)
                print_tree(NO_MESSAGE, 2*i + 2);
            cout << setw(depth*3) << "";
            cout << heap[i].data;
            cout << '(' << heap[i].priority << ')' << endl;
            if (2*i + 1 < used)
                print_tree(NO_MESSAGE, 2*i + 1);
        }
    }
    
    void p_queue::print_array(const char message[]) const
    // Pre:  (none)
    // Post: If the message is non-empty, it has first been written to
    //       cout. After that, the contents of the array representing
    //       the current heap has been written to cout in one line with
    //       values separated one from another with a space.
    //       NOTE: The default argument for message is the empty string.
    {
        if (message[0] != '\0')
            cout << message << endl;
        
        if (used == 0)
            cout << "(EMPTY)" << endl;
        else
            for (size_type i = 0; i < used; i++)
                cout << heap[i].data << ' ';
    }
    
    // CONSTRUCTOR for the p_queue class:
    //   p_queue(size_type initial_capacity = DEFAULT_CAPACITY)
    //     Pre:  initial_capacity > 0
    //     Post: The p_queue has been initialized to an empty p_queue.
    //       The push function will work efficiently (without allocating
    //       new memory) until this capacity is reached.
    //     Note: If Pre is not met, initial_capacity will be adjusted to
    //       DEFAULT_CAPACITY. I.e., when creating a p_queue object,
    //       client can override initial_capacity with something deemed
    //       more appropriate than DEFAULT_CAPACITY; but if (in doing so)
    //       client mis-specifies 0 (NOTE: size_type is unsigned, thus
    //       can't be negative) as the overriding size, DEFAULT_CAPACITY
    //       remains as the value to be used for initial_capacity (this
    //       is to ensure no attempt is made at allocating memory that's
    //       0 in amount).
    
    // DEFAULT CONSTRUCTOR DONE
    p_queue::p_queue(size_type initial_capacity) : capacity(initial_capacity), used(0)
    {
        // pre condition
        if (capacity < 1)
            capacity = DEFAULT_CAPACITY;
        
        // allocate the new array
        heap = new ItemType [capacity];
    }
    
    // COPY CONSTRUCTOR DONE
    p_queue::p_queue(const p_queue& src) : capacity(src.capacity), used(src.used)
    {
        // declare the new int array
        heap = new ItemType [src.capacity];
        
        // set the data of the array
        memcpy(heap, src.heap, sizeof(src.heap) * used);
    }
    
    // DESTRUCTOR DONE
    p_queue::~p_queue()
    {
        delete [] heap;
    }
    
    // MODIFICATION MEMBER FUNCTIONS
    // ASSIGNMENT OPERATOR DONE!
    p_queue& p_queue::operator=(const p_queue& rhs)
    {
        if(&rhs != this)
        {
            // set used and capacity to the rhs capacity.
            used = rhs.used;
            capacity = rhs.capacity;
            
            // declare a new data array.
            ItemType *newHeap = new ItemType [rhs.capacity];
            
            // copy values from rhs to new array
            memcpy(newHeap, rhs.heap, sizeof(rhs.heap) * used);
            
            // delete the existing array
            delete heap;
            
            // assign the pointer to the new Array
            heap = newHeap;
        }
        return *this;
    }
    
    //   void push(const value_type& entry, size_type priority)
    //     Pre:  (none)
    //     Post: A new copy of item with the specified data and priority
    //           has been added to the p_queue.
    //
    // PUSH FUNCTION WORKS TESTED!
    void p_queue::push(const value_type& entry, size_type priority)
    {
        // check if the array has capacity to add entry
        if(used >= capacity)
        {
            resize(capacity * 1.5);
        }
        // Declare a new Itemtype
        ItemType newItem;
        
        // Assign newItem data fields
        newItem.priority = priority;
        newItem.data = entry;
        
        // insert the data into the lowest place in the array.
        heap[used] = newItem;
        
        // temp value to hold array index of next element.
        size_type temp = used;
        
        if(used == 0)
        {
            used++;
            return;
        }
        else
            used++;
        
        // If the childs priority is greater than the parents
        // Issue only works for one itteration.
        while(temp != 0 && parent_priority(temp) < heap[temp].priority)
        {
            // swap with parent
            swap_with_parent(temp);
            
            // update temp to new index location
            temp = parent_index(temp);
        }
        // from this possition we are at the root.
    }
    
    //   void pop()
    //     Pre:  size() > 0.
    //     Post: The highest priority item has been removed from the
    //           p_queue. (If several items have the equal priority,
    //           then the implementation may decide which one to remove.)
    //
    // POP FUNCTION NOT IMPLEMENTED!!
    void p_queue::pop()
    {
        // pre condition
        assert(size() > 0);
        //if ( used == 1 )
        //    used--;
        //else
        //{
            size_type tempIndex = 0;
            // copy the deepest entry in the tree into the root.
            heap[tempIndex] = heap[used - 1];
        
            while((!is_leaf(tempIndex)) && big_child_priority(tempIndex) > heap[tempIndex].priority)
            {
                // swap with child value, big_child_index
                // find the big childs index
                size_type temp = big_child_index(tempIndex);
                swap_with_parent(big_child_index(tempIndex));
                tempIndex = temp;
                // swap function
                //ItemType holder = heap[tempIndex];
                //heap[tempIndex] = heap[temp];
                //heap[temp] = holder;
            }
            used--;
        //}
    }
    
    p_queue::size_type p_queue::size() const
    {
        return used;
    }
    
    bool p_queue::empty() const
    {
        return used == 0;
    }
    
    //   value_type front() const
    //     Pre:  size() > 0.
    //     Post: The return value is the data of the highest priority
    //           item in the p_queue, but the p_queue is unchanged.
    //           (If several items have equal priority, then the
    //           implementation may decide which one to return.)
    // FRONT FUNCTION DONE!
    p_queue::value_type p_queue::front() const
    {
        // ensure there is a value stored in the array.
        assert(size() > 0);
        
        // if there is, return the root node
        // in heap root node will always be highest priority.
        return heap[0].data;
    }
    
    // PRIVATE HELPER FUNCTIONS
    // RESIZE DONE!
    void p_queue::resize(size_type new_capacity)
    {
        // if the new_capacity is equal to the old capacity increment new_capacity + 1
        if(new_capacity == capacity)
            new_capacity++;
        
        // if the new capacity is less than old capacity
        if(new_capacity < capacity)
            new_capacity = capacity;
        
        // if the capacity < 1 then assign new capacity to 1
        if(new_capacity < 1)
            new_capacity = 1;
        
        capacity = new_capacity;
        
        ItemType *temp = new ItemType [capacity];
        
        for(size_type i = 0; i < used; i++)
            temp[i] = heap[i];
        
        delete [] heap;
        
        heap = temp;
    }
    
    bool p_queue::is_leaf(size_type i) const
    // Pre:  (i < used)
    // Post: If the item at heap[i] has no children, true has been
    //       returned, otherwise false has been returned.
    {
        assert(i < used);
        // return if there is a left or right child.
        if ((2 * i + 1) < used || (2 * i + 2) < used)
            return false;
        else
            return true;
    }
    /*{
        assert(i < used);
        if (((2*i) + 1) >= used)
            return true;
        return false;
    }*/
    
    // PARENT INDEX FUNCTION DONE!
    p_queue::size_type p_queue::parent_index(size_type i) const
    // Pre:  (i > 0) && (i < used)
    // Post: The index of "the parent of the item at heap[i]" has
    //       been returned.
    {
        assert(i > 0 && i < used);
        return (i - 1) / 2;
    }
    
    p_queue::size_type p_queue::parent_priority(size_type i) const
    // Pre:  (i > 0) && (i < used)
    // Post: The priority of "the parent of the item at heap[i]" has
    //       been returned.
    {
        // assert value passed is valid
        assert(i > 0 && i < used);
        return heap[(i - 1) / 2].priority;
    }
    
    // RETURN THE LARGER OF THE 2 CHILD NODES DONE!
    p_queue::size_type p_queue::big_child_index(size_type i) const
    // Pre:  is_leaf(i) returns false
    // Post: The index of "the bigger child of the item at heap[i]"
    //       has been returned.
    //       (The bigger child is the one whose priority is no smaller
    //       than that of the other child, if there is one.)
    {
        // assert the node has children.
        assert(is_leaf(i) == false);
        
        // check if it has a right child
        if(2 * i + 2 < used)
        {
            // if the left child is greater than the right child.
            if(heap[2 * i + 1].priority >= heap[2 * i + 2].priority)
                return 2 * i + 1;
            
            // else the right child is greater
            else
                return 2 * i + 2;
        }
        // else it only has a left child
        else
            return 2 * i + 1;
    }
    
    // PRIORITY OF BIG CHILD
    p_queue::size_type p_queue::big_child_priority(size_type i) const
    // Pre:  is_leaf(i) returns false
    // Post: The priority of "the bigger child of the item at heap[i]"
    //       has been returned.
    //       (The bigger child is the one whose priority is no smaller
    //       than that of the other child, if there is one.)
    {
        // assert the node has children
        assert( is_leaf(i) == false );
        
        // check if it has a right child
        if(2 * i + 2 < used)
        {
            // if the left childs priority is greater than the right child.
            if(heap[2 * i + 1].priority > heap[2 * i + 2].priority)
                return heap[2 * i + 1].priority;
            
            // else the right child is greater
            else
                return heap[2 * i + 2].priority;
        }
        // else it only has a left child
        else
            return heap[2 * i + 1].priority;
    }
    
    // SWAP FUNCTION WITH PARENT
    void p_queue::swap_with_parent(size_type i)
    // Pre:  (i > 0) && (i < used)
    // Post: The item at heap[i] has been swapped with its parent.
    {
        // assert that the value passed is valid.
        assert( (i > 0) && (i < used) );
        
        // declare temp variable to hold child
        ItemType temp = heap[i];
        
        // set child node value to the parent
        heap[i] = heap[(i - 1) / 2];
        
        // set the parent node value to the temp
        heap[(i - 1) / 2] = temp;
    }
}

