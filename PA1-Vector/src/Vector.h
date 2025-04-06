#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <type_traits> // std::is_same

template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow() {
        size_t newCap;
        if(_capacity == 0) newCap = 1;
        else newCap = _capacity * 2;

        T* newArr = new T[newCap];
        for(size_t i = 0; i < _size; ++i) {
            newArr[i] = std::move(array[i]);
        }
        delete[] array;
        array = newArr;
        _capacity = newCap;
    }

public:
    Vector() noexcept : array(nullptr), _capacity(0), _size(0) {}
    Vector(size_t count, const T& value) : _capacity(count), _size(count) {
        array = new T[count];
        for(size_t i = 0; i < count; ++i) {
            array[i] = value;
        }
    }
    explicit Vector(size_t count) : _capacity(count), _size(count) {
        array = new T[count]();
    }
    Vector(const Vector& other) : _capacity(other._capacity), _size(other._size) {
        array = new T[_capacity];
        for(size_t i = 0; i < _size; ++i) {
            array[i] = other.array[i];
        }
    }
    Vector(Vector&& other) noexcept : _capacity(other._capacity), _size(other._size) {
        array = other.array;
        other.array = nullptr;
        other._capacity = 0;
        other._size = 0;
    }

    ~Vector() {
        clear();
    }

    Vector& operator=(const Vector& other) {
        if(this == &other) return *this;
        clear();
        array = new T[other._capacity];
        _capacity = other._capacity;
        _size = other._size;
        for(size_t i = 0; i < _size; ++i) {
            array[i] = other.array[i];
        }
        return *this;
    }
    Vector& operator=(Vector&& other) noexcept {
        if(this == &other) return *this;
        delete[] array;
        array = other.array;
        _capacity = other._capacity;
        _size = other._size;
        other.array = nullptr;
        other._capacity = 0;
        other._size = 0;
        return *this;
    }

    iterator begin() noexcept {
        return iterator(&array[0]);
    }
    iterator end() noexcept {
        return iterator(&array[_size]);
    }

    [[nodiscard]] bool empty() const noexcept {
        return (_size == 0);
    }
    size_t size() const noexcept { 
        return _size; 
    }
    size_t capacity() const noexcept { 
        return _capacity; 
    }

    T& at(size_t pos) {
        if(pos < 0 || pos >= _size) throw std::out_of_range("");
        return array[pos];
    }
    const T& at(size_t pos) const {
        if(pos < 0 || pos >= _size) throw std::out_of_range("");
        return array[pos];
    }
    T& operator[](size_t pos) {
        return array[pos];
    }
    const T& operator[](size_t pos) const {
        return array[pos];
    }
    T& front() {
        return array[0];
    }
    const T& front() const {
        return array[0];
    }
    T& back() {
        return array[_size - 1];
    }
    const T& back() const {
        return array[_size - 1];
    }

    void push_back(const T& value) {
        if(_size == _capacity) grow();
        array[_size] = value;
        _size++;
    }
    void push_back(T&& value) {
        if(_size == _capacity) grow();
        array[_size] = std::move(value);
        _size++;
    }
    void pop_back() {
        array[_size - 1] = T();
        _size--;
    }

    iterator insert(iterator pos, const T& value) {
        ptrdiff_t index = pos - begin();
        if(_size == _capacity) grow();
        for(ptrdiff_t i = _size; i > index; --i) {
            array[i] = std::move(array[i - 1]);
        }
        array[index] = value;
        _size++;
        return iterator(&array[index]);
    }
    iterator insert(iterator pos, T&& value) {
        ptrdiff_t index = pos - begin();
        if(_size == _capacity) grow();
        for(ptrdiff_t i = _size; i > index; --i) {
            array[i] = std::move(array[i - 1]);
        }
        array[index] = std::move(value);
        _size++;
        return iterator(&array[index]);
    }
    iterator insert(iterator pos, size_t count, const T& value) {
        ptrdiff_t index = pos - begin();
        while(_size + count >= _capacity) grow();
        for(size_t i = _size + count - 1; i >= index + count; --i) {
            array[i] = std::move(array[i - count]);
        }
        for(size_t i = 0; i < count; ++i) {
            array[index + i] = value;
        }
        _size += count;
        return iterator(&array[index]);
    }
    iterator erase(iterator pos) {
        ptrdiff_t index = pos - begin();
        for(size_t i = index; i < _size - 1; ++i) {
            array[i] = std::move(array[i + 1]);
        }
        _size--;
        return iterator(&array[index]);
    }
    iterator erase(iterator first, iterator last) {
        ptrdiff_t startIndex = first - begin();
        ptrdiff_t endIndex = last - begin();
        ptrdiff_t count = endIndex - startIndex;
        for(size_t i = startIndex; i < _size - count; ++i) {
            array[i] = std::move(array[i + count]);
        }
        _size -= count;
        return iterator(&array[startIndex]);
    }


    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        // Points to some element in the vector (or nullptr)
        T* _ptr;

    public:
        iterator() {
            _ptr = nullptr;
        }
        explicit iterator(T* ptr) {
            _ptr = ptr;
        }

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        [[nodiscard]] reference operator*() const noexcept {
            return *_ptr;
        }
        [[nodiscard]] pointer operator->() const noexcept {
            return _ptr;
        }

        // Prefix Increment: ++a
        iterator& operator++() noexcept {
            _ptr++;
            return *this;
        }
        // Postfix Increment: a++
        iterator operator++(int) noexcept {
            iterator temp = *this;
            _ptr++;
            return temp;
        }
        // Prefix Decrement: --a
        iterator& operator--() noexcept {
            _ptr--;
            return *this;
        }
        // Postfix Decrement: a--
        iterator operator--(int) noexcept {
            iterator temp = *this;
            _ptr--;
            return temp;
        }

        iterator& operator+=(difference_type offset) noexcept {
            _ptr += offset;
            return *this;
        }
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept {
            return iterator(_ptr + offset);
        }
        
        iterator& operator-=(difference_type offset) noexcept {
            _ptr -= offset;
            return *this;
        }
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept {
            return iterator(_ptr - offset);
        }
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept {
            return (_ptr - rhs._ptr);
        }

        [[nodiscard]] reference operator[](difference_type offset) const noexcept {
            return *(_ptr + offset);
        }

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept { return (_ptr == rhs._ptr); }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept { return (_ptr != rhs._ptr); }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept { return (_ptr < rhs._ptr); }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept { return (_ptr > rhs._ptr); }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept { return (_ptr <= rhs._ptr); }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept { return (_ptr >= rhs._ptr); }
    };


    void clear() noexcept {
        delete[] array;
        array = nullptr;
        _size = 0;
    }
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept {
    _Iterator temp = iterator;
    if(offset > 0) {
        for(ptrdiff_t i = 0; i < offset; ++i) {
            temp++;
        }
    }
    else if(offset < 0) {
        ptrdiff_t absval = offset * -1;
        for(ptrdiff_t i = 0; i < absval; ++i) {
            temp--;
        }
    }
    return temp;
}

#endif