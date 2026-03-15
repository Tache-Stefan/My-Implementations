#pragma once

#include <cstddef>
#include <iterator>
#include <limits>
#include <utility>
#include <type_traits>

namespace my_std {

    template<typename T>
    class List {
        private:
            struct Node;

        public:
            class iterator {
                public:
                    using iterator_category = std::bidirectional_iterator_tag;
                    using value_type        = T;
                    using difference_type   = std::ptrdiff_t;
                    using pointer           = T*;
                    using reference         = T&;

                    iterator() = default;

                    reference operator*() const { return m_node->data; }
                    pointer operator->() const { return &m_node->data; }

                    iterator& operator++() {
                        if (m_node) { m_node = m_node->next; }
                        return *this;
                    }
                    iterator operator++(int) {
                        iterator temp = *this;
                        ++(*this);
                        return temp;
                    }
                    iterator& operator--() {
                        if (m_node) {
                            m_node = m_node->prev;
                        } else {
                            m_node = m_owner->m_tail;
                        }
                        return *this;
                    }
                    iterator operator--(int) {
                        iterator temp = *this;
                        --(*this);
                        return temp;
                    }

                    friend bool operator==(const iterator& a, const iterator& b) {
                        return a.m_node == b.m_node;
                    }
                    friend bool operator!=(const iterator& a, const iterator& b) {
                        return !(a == b);
                    }
                
                private:
                    using NodePtr = Node*;
                    NodePtr m_node = nullptr;
                    List* m_owner = nullptr;

                    iterator(NodePtr node, List* owner) : m_node(node), m_owner(owner) {}
                    friend class List;
                    friend class const_iterator;
            };

            class const_iterator {
                public:
                    using iterator_category = std::bidirectional_iterator_tag;
                    using value_type        = T;
                    using difference_type   = std::ptrdiff_t;
                    using pointer           = const T*;
                    using reference         = const T&;

                    const_iterator() = default;
                    const_iterator(const iterator& it) : m_node(it.m_node), m_owner(it.m_owner) {}

                    reference operator*() const { return m_node->data; }
                    pointer operator->() const { return &m_node->data; }

                    const_iterator& operator++() {
                        if (m_node) { m_node = m_node->next; }
                        return *this;
                    }
                    const_iterator operator++(int) {
                        const_iterator temp = *this;
                        ++(*this);
                        return temp;
                    }
                    const_iterator& operator--() {
                        if (m_node) {
                            m_node = m_node->prev;
                        } else {
                            m_node = m_owner->m_tail;
                        }
                        return *this;
                    }
                    const_iterator operator--(int) {
                        const_iterator temp = *this;
                        --(*this);
                        return temp;
                    }

                    friend bool operator==(const const_iterator& a, const const_iterator& b) {
                        return a.m_node == b.m_node;
                    }
                    friend bool operator!=(const const_iterator& a, const const_iterator& b) {
                        return !(a == b);
                    }
                
                private:
                    using NodePtr = const Node*;
                    NodePtr m_node = nullptr;
                    const List* m_owner = nullptr;

                    const_iterator(NodePtr node, const List* owner) : m_node(node), m_owner(owner) {}
                    friend class List;
            };

            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            List() = default;

            void push_back(const T& value);
            void push_back(T&& value);
            template<typename... Args>
            void emplace_back(Args&&... args);

            void push_front(const T& value);
            void push_front(T&& value);
            template<typename... Args>
            void emplace_front(Args&&... args);

            void pop_back() noexcept(std::is_nothrow_destructible_v<T>);
            void pop_front() noexcept(std::is_nothrow_destructible_v<T>);

            void resize(const size_t count);
            void resize(const size_t count, const T& value);
            void swap(List& other) noexcept;

            inline T& back() noexcept { return m_tail->data; }
            inline const T& back() const noexcept { return m_tail->data; } 
            inline T& front() noexcept { return m_head->data; }
            inline const T& front() const noexcept { return m_head->data; }

            inline bool empty() const noexcept { return m_size == 0; }
            inline size_t size() const noexcept { return m_size; }
            inline size_t max_size() const noexcept { return std::numeric_limits<size_t>::max(); }

            void clear() noexcept(std::is_nothrow_destructible_v<T>);

            iterator begin() noexcept { return iterator(m_head, this); }
            iterator end() noexcept { return iterator(nullptr, this); }
            const_iterator begin() const noexcept { return const_iterator(m_head, this); }
            const_iterator end() const noexcept { return const_iterator(nullptr, this); }
            const_iterator cbegin() const noexcept { return const_iterator(m_head, this); }
            const_iterator cend() const noexcept { return const_iterator(nullptr, this); }
            reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
            reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
            const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
            const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
            const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
            const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

            List(const List& other);
            List& operator=(const List& other);

            List(List&& other) noexcept(std::is_nothrow_destructible_v<T>);
            List& operator=(List&& other) noexcept(std::is_nothrow_destructible_v<T>);

            ~List() { clear(); }

        private:
            struct Node {
                T data;
                Node* prev;
                Node* next;

                template<typename... Args>
                Node(Args&&... args) 
                    : data(std::forward<Args>(args)...), prev(nullptr), next(nullptr) {}
            };

            Node* m_head = nullptr;
            Node* m_tail = nullptr;
            size_t m_size = 0;
    };

    template<typename T>
    void List<T>::push_back(const T& value) {
        emplace_back(value);
    }

    template<typename T>
    void List<T>::push_back(T&& value) {
        emplace_back(std::move(value));
    }

    template<typename T>
    template<typename... Args>
    void List<T>::emplace_back(Args&&... args) {
        Node* newNode = new Node(std::forward<Args>(args)...);

        if (!m_head) {
            m_head = m_tail = newNode;
        } else {
            m_tail->next = newNode;
            newNode->prev = m_tail;
            m_tail = newNode;
        }

        ++m_size;
    }

    template<typename T>
    void List<T>::push_front(const T& value) {
        emplace_front(value);
    }

    template<typename T>
    void List<T>::push_front(T&& value) {
        emplace_front(std::move(value));
    }

    template<typename T>
    template<typename... Args>
    void List<T>::emplace_front(Args&&... args) {
        Node* newNode = new Node(std::forward<Args>(args)...);

        if (!m_head) {
            m_head = m_tail = newNode;
        } else {
            m_head->prev = newNode;
            newNode->next = m_head;
            m_head = newNode;
        }

        ++m_size;
    }

    template<typename T>
    void List<T>::pop_back() noexcept(std::is_nothrow_destructible_v<T>) {
        if (empty()) return;
        if (m_size == 1) {
            clear();
            return;
        }

        Node* toDelete = m_tail;
        m_tail = m_tail->prev;
        m_tail->next = nullptr;
        delete toDelete;
        --m_size;
    }

    template<typename T>
    void List<T>::pop_front() noexcept(std::is_nothrow_destructible_v<T>) {
        if (empty()) return;
        if (m_size == 1) {
            clear();
            return;
        }

        Node* toDelete = m_head;
        m_head = m_head->next;
        m_head->prev = nullptr;
        delete toDelete;
        --m_size;
    }

    template<typename T>
    void List<T>::resize(const size_t count) {
        while (m_size > count) {
            pop_back();
        }
        while (m_size < count) {
            emplace_back();
        }
    }

    template<typename T>
    void List<T>::resize(const size_t count, const T& value) {
        while (m_size > count) {
            pop_back();
        }
        while (m_size < count) {
            emplace_back(value);
        }
    }

    template<typename T>
    void List<T>::swap(List& other) noexcept {
        Node* head = m_head;
        m_head = other.m_head;
        other.m_head = head;

        Node* tail = m_tail;
        m_tail = other.m_tail;
        other.m_tail = tail;

        size_t size = m_size;
        m_size = other.m_size;
        other.m_size = size;
    }

    template<typename T>
    List<T>::List(const List& other)
        : m_head(nullptr), m_tail(nullptr), m_size(0) {
        try {
            for (Node* current = other.m_head; current != nullptr; current = current->next) {
                push_back(current->data);
            }
        } catch (...) {
            clear();
            throw;
        }
    }
    
    template<typename T>
    List<T>& List<T>::operator=(const List& other) {
        if (this == &other) return *this;

        List temp(other);
        swap(temp);
        return *this;
    }

    template<typename T>
    List<T>::List(List&& other) noexcept(std::is_nothrow_destructible_v<T>)
        : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size) {
        other.m_head = other.m_tail = nullptr;
        other.m_size = 0;
    }

    template<typename T>
    List<T>& List<T>::operator=(List&& other) noexcept(std::is_nothrow_destructible_v<T>) {
        if (this == &other) return *this;

        clear();
        m_head = other.m_head;
        m_tail = other.m_tail;
        m_size = other.m_size;

        other.m_head = nullptr;
        other.m_tail = nullptr;
        other.m_size = 0;

        return *this;
    }

    template<typename T>
    void List<T>::clear() noexcept(std::is_nothrow_destructible_v<T>) {
        Node* current = m_head;

        while (current) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }

        m_head = m_tail = nullptr;
        m_size = 0;
    }

}
