#ifndef SINGLY_LIST_HPP
#define SINGLY_LIST_HPP

#include <memory_resource>
#include <cstddef>
#include <memory> 

template <typename T>
class SinglyList {
    struct Node {
        T value;
        Node* next;
    };

    std::pmr::polymorphic_allocator<Node> alloc;
    Node* head = nullptr;
    Node* tail = nullptr;
    size_t count = 0;

public:
    explicit SinglyList(std::pmr::memory_resource* r = std::pmr::get_default_resource())
        : alloc(r) {
    }

    ~SinglyList() { clear(); }

    void push_back(const T& val) {
        Node* n = alloc.allocate(1);
        std::allocator_traits<decltype(alloc)>::construct(alloc, n, Node{ val, nullptr });
        if (!head) head = tail = n;
        else { tail->next = n; tail = n; }
        ++count;
    }

    void clear() {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            std::allocator_traits<decltype(alloc)>::destroy(alloc, cur);
            alloc.deallocate(cur, 1);
            cur = nxt;
        }
        head = tail = nullptr;
        count = 0;
    }

    size_t size() const { return count; }

    class iterator {
        Node* ptr;
    public:
        iterator(Node* n = nullptr) : ptr(n) {}
        T& operator*() const { return ptr->value; }
        T* operator->() const { return &ptr->value; }
        iterator& operator++() { ptr = ptr->next; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
    };

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
};

#endif 
