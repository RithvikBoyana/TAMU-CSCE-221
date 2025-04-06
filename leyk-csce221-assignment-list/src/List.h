#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag
#include <type_traits> // std::is_same, std::enable_if

template <class T>
class List {
    private:
    struct Node {
        Node *next, *prev;
        T data;
        explicit Node(Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev} {}
        explicit Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{data} {}
        explicit Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template <typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = pointer_type;
        using reference         = reference_type;
    private:
        friend class List<value_type>;
        using Node = typename List<value_type>::Node;

        Node* node;

        explicit basic_iterator(Node* ptr) noexcept : node{ptr} {}
        explicit basic_iterator(const Node* ptr) noexcept : node{const_cast<Node*>(ptr)} {}

    public:
        basic_iterator() : node{nullptr} {};
        basic_iterator(const basic_iterator&) = default;
        basic_iterator(basic_iterator&&) = default;
        ~basic_iterator() = default;
        basic_iterator& operator=(const basic_iterator&) = default;
        basic_iterator& operator=(basic_iterator&&) = default;

        reference operator*() const {
            return node->data;
        }
        pointer operator->() const {
            return &node->data;
        }

        // Prefix Increment: ++a
        basic_iterator& operator++() {
            node = node->next;
            return *this;
        }
        // Postfix Increment: a++
        basic_iterator operator++(int) {
            auto temp = *this;
            node = node->next;
            return temp;
        }
        // Prefix Decrement: --a
        basic_iterator& operator--() {
            node = node->prev;
            return *this;
        }
        // Postfix Decrement: a--
        basic_iterator operator--(int) {
            auto temp = *this;
            node = node->prev;
            return temp;
        }

        bool operator==(const basic_iterator& other) const noexcept {
            return (node == other.node);
        }
        bool operator!=(const basic_iterator& other) const noexcept {
            return (node != other.node);
        }
    };

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = basic_iterator<pointer, reference>;
    using const_iterator  = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:
    List() : head(), tail(), _size(0) {
        head.next = &tail;
        tail.prev = &head;
    }
    List( size_type count, const T& value ) : List() {
        Node* temp = &head;
        for(size_type i = 0; i < count; ++i) {
            Node* newNode = new Node(value, temp, temp->next);
            temp->next = newNode;
            tail.prev = newNode;
            temp = newNode;
            _size++;
        }
    }
    explicit List( size_type count ) : List() {
        Node* temp = &head;
        for(size_type i = 0; i < count; ++i) {
            Node* newNode = new Node(T(), temp, temp->next);
            temp->next = newNode;
            tail.prev = newNode;
            temp = newNode;
            _size++;
        }
    }
    List( const List& other ) : List() {
        if(!other.empty()) {
            Node* tempThis = &head;
            Node* tempOther = other.head.next;
            while(tempOther != &other.tail) {
                Node* newNode = new Node(tempOther->data, tempThis, tempThis->next);
                tempThis->next = newNode;
                tempThis = tempThis->next;
                tempOther = tempOther->next;
                _size++;
            }
            tempThis->next = &tail;
            tail.prev = tempThis;
        }
    }
    List( List&& other ) : List() {
        if(!other.empty()) {
            Node* firstNode = other.head.next;
            Node* lastNode = other.tail.prev;
            head.next = firstNode;
            firstNode->prev = &head;
            tail.prev = lastNode;
            lastNode->next = &tail;
            other.head.next = &other.tail;
            other.tail.prev = &other.head;
            _size = other._size;
            other._size = 0;
        }
    }
    ~List() {
        this->clear();
    }
    List& operator=( const List& other ) {
        if(this != &other) {
            this->clear();
            if(!other.empty()) {
                Node *tempThis = &head;
                Node *tempOther = other.head.next;
                while (tempOther != &other.tail) {
                    Node *newNode = new Node(tempOther->data, tempThis, tempThis->next);
                    tempThis->next = newNode;
                    tempThis = tempThis->next;
                    tempOther = tempOther->next;
                    _size++;
                }
                tempThis->next = &tail;
                tail.prev = tempThis;
            }
        }
        return *this;
    }
    List& operator=( List&& other ) noexcept {
        if(this != &other) {
            this->clear();
            if(!other.empty()) {
                Node* firstNode = other.head.next;
                Node* lastNode = other.tail.prev;
                head.next = firstNode;
                firstNode->prev = &head;
                tail.prev = lastNode;
                lastNode->next = &tail;
                other.head.next = &other.tail;
                other.tail.prev = &other.head;
                _size = other._size;
                other._size = 0;
            }
        }
        return *this;
    }

    reference front() {
        return head.next->data;
    }
    const_reference front() const {
        return head.next->data;
    }
	
    reference back() {
        return tail.prev->data;
    }
    const_reference back() const {
        return tail.prev->data;
    }
	
    iterator begin() noexcept {
        return iterator(head.next);
    }
    const_iterator begin() const noexcept {
        return const_iterator(head.next);
    }
    const_iterator cbegin() const noexcept {
        return const_iterator(head.next);
    }

    iterator end() noexcept {
        return iterator(&tail);
    }
    const_iterator end() const noexcept {
        return const_iterator(&tail);
    }
    const_iterator cend() const noexcept {
        return const_iterator(&tail);
    }

    bool empty() const noexcept {
        return (head.next == &tail);
    }

    size_type size() const noexcept {
        return _size;
    }

    void clear() noexcept {
        if(!this->empty()) {
            Node *temp = head.next;
            while (temp != &tail) {
                Node *nextNode = temp->next;
                delete temp;
                temp = nextNode;
            }
            head.next = &tail;
            tail.prev = &head;
            _size = 0;
        }
    }

    iterator insert( const_iterator pos, const T& value ) {
        Node* currNode = pos.node;
        Node* prevNode = currNode->prev;
        Node* newNode = new Node(value, prevNode, currNode);
        prevNode->next = newNode;
        currNode->prev = newNode;
        ++_size;
        return iterator(newNode);
    }
    iterator insert( const_iterator pos, T&& value ) {
        Node* currNode = pos.node;
        Node* prevNode = currNode->prev;
        Node* newNode = new Node(std::move(value), prevNode, currNode);
        prevNode->next = newNode;
        currNode->prev = newNode;
        ++_size;
        return iterator(newNode);
    }

    iterator erase( const_iterator pos ) {
        Node* currNode = pos.node;
        Node* nextNode = currNode->next;
        Node* prevNode = currNode->prev;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
        delete currNode;
        --_size;
        return iterator(nextNode);
    }

    void push_back( const T& value ) {
        Node* prevNode = tail.prev;
        Node* newNode = new Node(value, prevNode, &tail);
        prevNode->next = newNode;
        tail.prev = newNode;
        ++_size;
    }
    void push_back( T&& value ) {
        Node* prevNode = tail.prev;
        Node* newNode = new Node(std::move(value), prevNode, &tail);
        prevNode->next = newNode;
        tail.prev = newNode;
        ++_size;
    }

    void pop_back() {
        if(!this->empty()) {
            Node *currNode = tail.prev;
            Node *prevNode = currNode->prev;
            prevNode->next = &tail;
            tail.prev = prevNode;
            delete currNode;
            --_size;
        }
    }
	
    void push_front( const T& value ) {
        Node* nextNode = head.next;
        Node* newNode = new Node(value, &head, nextNode);
        head.next = newNode;
        nextNode->prev = newNode;
        ++_size;
    }
	void push_front( T&& value ) {
        Node* nextNode = head.next;
        Node* newNode = new Node(std::move(value), &head, nextNode);
        head.next = newNode;
        nextNode->prev = newNode;
        ++_size;
    }

    void pop_front() {
        if(!this->empty()) {
            Node* currNode = head.next;
            Node* nextNode = currNode->next;
            head.next = nextNode;
            nextNode->prev = &head;
            delete currNode;
            --_size;
        }
    }

    /*
      You do not need to modify these methods!
      
      These method provide the non-const complement 
      for the const_iterator methods provided above.
    */
    iterator insert( iterator pos, const T & value) { 
        return insert((const_iterator &) (pos), value);
    }

    iterator insert( iterator pos, T && value ) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase( iterator pos ) {
        return erase((const_iterator&)(pos));
    }
};


/*
    You do not need to modify these methods!

    These method provide a overload to compare const and 
    non-const iterators safely.
*/
 
namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
        std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::iterator, 
            Iter
        >{} && std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
            ConstIter
        >{}, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) != lhs;
}