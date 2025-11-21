#ifndef SINGLY_LIST_HPP
#define SINGLY_LIST_HPP

#include <memory_resource>
#include <cstddef>
#include <iterator>

template <typename T>
class SinglyList {
    struct Node { T value; Node* next; };
    using Alloc = std::pmr::polymorphic_allocator<Node>;
    using Traits = std::allocator_traits<Alloc>;

    Alloc alloc;
    Node* head = nullptr;
    Node* tail = nullptr;
    size_t sz = 0;

public:
    explicit SinglyList(std::pmr::memory_resource* r = std::pmr::get_default_resource())
        : alloc(r) {
    }

    ~SinglyList() { clear(); }

    void push_back(const T& v) {
        Node* n = Traits::allocate(alloc, 1);
        Traits::construct(alloc, n, Node{ v, nullptr });
        if (!head) head = tail = n;
        else { tail->next = n; tail = n; }
        ++sz;
    }

    void clear() {
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            Traits::destroy(alloc, cur);
            Traits::deallocate(alloc, cur, 1);
            cur = next;
        }
        head = tail = nullptr;
        sz = 0;
    }

    size_t size() const { return sz; }

    class iterator {
        Node* p;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;

        iterator(Node* n = nullptr) : p(n) {}
        reference operator*() const { return p->value; }
        pointer operator->() const { return &p->value; }
        iterator& operator++() { p = p->next; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const iterator& o) const { return p == o.p; }
        bool operator!=(const iterator& o) const { return p != o.p; }
    };

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
};

#endif 