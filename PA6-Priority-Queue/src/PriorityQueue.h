#pragma once

#include <functional>
#include <utility>
#include <vector>

template <class T, class Container = std::vector<T>, class Compare = std::less<T>>
class PriorityQueue {
public:
    using value_compare = Compare;
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;

private:
    Container c;
    value_compare comp;

    size_type parent(size_type index) { return (index - 1) / 2; }
    size_type left_child(size_type index) { return 2 * index + 1; }
    size_type right_child(size_type index) { return 2 * (index + 1); }
    bool is_internal(size_t index) { return left_child(index) < c.size(); }
    bool is_leaf(size_t index) { return left_child(index) >= c.size(); }

    
    /**
     * @brief Recursively (or Iteratively) swap the value at index up the heap until it is in the correct position.
     * 
     * @note This is a max heap, so assume Compare is less, but promote the larger value.
     * 
     * Used by push
     * 
     * O(log(size()))
     * 
     * @param index the current position to swap upwards
     */
    void upheap(size_type index) {
        if(index == 0) return;

        size_type p_index = parent(index);

        if(comp(c.at(p_index), c.at(index))) {
            std::swap(c.at(p_index), c.at(index));
            upheap(p_index);
        }
    }
    
    /**
     * @brief Recursively swap the value at index down the heap until it is in the correct position.
     * 
     * @note This is a max heap, so assume Compare is less, so promote the larger value.
     * 
     * Used by pop
     * 
     * O(log(size()))
     * 
     * @param index the current position to swap downwards
     */
    void downheap(size_type index) {
        if(is_leaf(index)) return;

        size_type l_index = left_child(index);
        size_type r_index = right_child(index);

        if(r_index < size()) {
            // Two children

            size_type greater_child = 0;
            if(comp(c.at(l_index), c.at(r_index))) greater_child = r_index;
            else greater_child = l_index;

            // Another way to compute greater_child using boolean casting to int (since these two indices differ by 1)
            // size_type = l_index + comp(c.at(l_index), c.at(r_index));

            if(comp(c.at(index), c.at(greater_child))) {
                std::swap(c.at(index), c.at(greater_child));
                downheap(greater_child);
            }
        }
        else {
            // One child
            if(comp(c.at(index), c.at(l_index))) {
                std::swap(c.at(index), c.at(l_index));
                downheap(l_index);
            }
        }
    }

public:
    PriorityQueue() = default;
    PriorityQueue( const PriorityQueue& other ) = default;
    PriorityQueue( PriorityQueue&& other ) = default;
    ~PriorityQueue() = default;
    PriorityQueue& operator=( const PriorityQueue& other ) = default;
    PriorityQueue& operator=( PriorityQueue&& other ) = default;

    /**
     * @brief Return a const reference to the element at the top of the heap.
     * 
     * O(1)
     * 
     * @return const_reference to the element at the top of the heap.
     */
    const_reference top() const { return c.at(0); }

    /**
     * @brief Return whether the heap is empty. This is the same as whether the underlying container, c, is empty.
     * 
     * O(1)
     * 
     * @return true c is empty
     * @return false c is not empty
     */
    bool empty() const { return c.empty(); }

    /**
     * @brief Return the number of elements in the heap. This is the same as the number of elements in the underlying container, c.
     * 
     * O(1)
     * 
     * @return size_type of the number of elements in the heap
     */
    size_type size() const { return c.size(); }
	
    /**
     * @brief Inserts element and sorts the underlying container, c.
     * 
     * Uses upheap
     * 
     * O(log(size())) 
     * 
     * @param value inserted by copying into c 
     */
    void push( const value_type& value ) {
        c.push_back(value);
        upheap(size() - 1);
    }

    /**
     * @brief Inserts element and sorts the underlying container, c.
     * 
     * Uses upheap
     * 
     * O(log(size())) 
     * 
     * @param value inserted by moving into c 
     */
	void push( value_type&& value ) {
        c.push_back(std::move(value));
        upheap(size() - 1);
    }

    /**
     * @brief Removes the top element.
     * 
     * Uses downheap
     * 
     * O(log(size())) 
     * 
     */
    void pop() {
        if(empty()) return;

        std::swap(c.at(0), c.at(size() - 1));
        c.pop_back();
        downheap(0);
    }
};