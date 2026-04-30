#pragma once

#include <cstdint>
#include <vector>
#include <stdexcept>

template<typename K, typename V>
struct Hash_Node {
    K key;
    V value;
    Hash_Node* next = nullptr;

    Hash_Node(const K& key, const V& value) : key(key), value(value) {}
};

template<typename K, typename V>
class Unordered_Map {
public:
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = Hash_Node<K, V>;
        using difference_type   = std::ptrdiff_t;
        using pointer           = Hash_Node<K, V>*;
        using reference         = Hash_Node<K, V>&;

        iterator() = default;

        reference operator*() const { return *_node; }
        pointer operator->() const { return _node; }

        iterator& operator++() {
            if (_node) {
                _node = _node->next;
            }

            if (!_node) {
                ++_bucket_idx;
                while (_bucket_idx < _owner->_capacity && !_owner->_buckets[_bucket_idx]) {
                    ++_bucket_idx;
                }

                if (_bucket_idx < _owner->_capacity) {
                    _node = _owner->_buckets[_bucket_idx];
                } else {
                    _node = nullptr;
                }
            }

            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        friend bool operator==(const iterator& lhs, const iterator& rhs) {
            return lhs._node == rhs._node;
        }

        friend bool operator!=(const iterator& lhs, const iterator& rhs) {
            return !(lhs == rhs);
        }

    private:
        using NodePtr = Hash_Node<K, V>*;
        NodePtr _node = nullptr;
        Unordered_Map* _owner = nullptr;
        size_t _bucket_idx = 0;

        iterator(NodePtr node, Unordered_Map* owner, size_t bucket_idx)
            : _node(node), _owner(owner), _bucket_idx(bucket_idx) {}

        friend class Unordered_Map;
        friend class const_iterator;
    };

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = const Hash_Node<K, V>;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const Hash_Node<K, V>*;
        using reference         = const Hash_Node<K, V>&;

        const_iterator() = default;
        const_iterator(const iterator& it)
            : _node(it._node), _owner(it._owner), _bucket_idx(it._bucket_idx) {}

        reference operator*() const { return *_node; }
        pointer operator->() const { return _node; }

        const_iterator& operator++() {
            if (_node) {
                _node = _node->next;
            }

            if (!_node) {
                ++_bucket_idx;
                while (_bucket_idx < _owner->_capacity && !_owner->_buckets[_bucket_idx]) {
                    ++_bucket_idx;
                }

                if (_bucket_idx < _owner->_capacity) {
                    _node = _owner->_buckets[_bucket_idx];
                } else {
                    _node = nullptr;
                }
            }

            return *this;
        }

        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }

        friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
            return lhs._node == rhs._node;
        }

        friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
            return !(lhs == rhs);
        }

    private:
        using NodePtr = const Hash_Node<K, V>*;
        NodePtr _node = nullptr;
        const Unordered_Map* _owner = nullptr;
        size_t _bucket_idx = 0;

        const_iterator(NodePtr node, const Unordered_Map* owner, size_t bucket_idx)
            : _node(node), _owner(owner), _bucket_idx(bucket_idx) {}

        friend class Unordered_Map;
    };

    iterator begin() {
        for (size_t i = 0; i < _capacity; ++i) {
            if (_buckets[i]) {
                return iterator(_buckets[i], this, i);
            }
        }
        return end();
    }

    iterator end() {
        return iterator(nullptr, this, _capacity);
    }

    const_iterator begin() const {
        for (size_t i = 0; i < _capacity; ++i) {
            if (_buckets[i]) {
                return const_iterator(_buckets[i], this, i);
            }
        }
        return end();
    }

    const_iterator end() const {
        return const_iterator(nullptr, this, _capacity);
    }

    Unordered_Map(size_t init_capacity = 1024) : _size(0), _capacity(round_power_of_two(init_capacity)) {
        _buckets.resize(_capacity, nullptr);
    }

    Unordered_Map(const Unordered_Map& other) : _size(other._size), _capacity(other._capacity) {
        _buckets.resize(_capacity, nullptr);
        for (size_t i = 0; i < _capacity; ++i) {
            Hash_Node<K, V>* head = other._buckets[i];
            Hash_Node<K, V>** curr = &_buckets[i];
            while (head) {
                *curr = new Hash_Node<K, V>(head->key, head->value);
                head = head->next;
                curr = &((*curr)->next);
            }
        }
    }
    Unordered_Map& operator=(const Unordered_Map& other) {
        if (this != &other) {
            Unordered_Map temp(other);
            std::swap(_buckets, temp._buckets);
            std::swap(_size, temp._size);
            std::swap(_capacity, temp._capacity);
        }
        return *this;
    }

    Unordered_Map(Unordered_Map&& other) noexcept
        : _buckets(std::move(other._buckets)), _size(other._size), _capacity(other._capacity) {
        other._size = 0;
        other._capacity = 0;
    }
    Unordered_Map& operator=(Unordered_Map&& other) noexcept {
        if (this != &other) {
            clear();

            _buckets = std::move(other._buckets);
            _size = other._size;
            _capacity = other._capacity;

            other._size = 0;
            other._capacity = 0;
        }
        return *this;
    }

    ~Unordered_Map() {
        for (size_t i = 0; i < _capacity; ++i) {
            Hash_Node<K, V>* head = _buckets[i];
            while (head) {
                Hash_Node<K, V>* temp = head;
                head = head->next;
                delete temp;
            }
        }
    }

    bool empty() const noexcept { return _size == 0; }
    size_t size() const noexcept { return _size; }

    void clear() {
        for (size_t i = 0; i < _capacity; ++i) {
            Hash_Node<K, V>* head = _buckets[i];
            while (head) {
                Hash_Node<K, V>* temp = head;
                head = head->next;
                delete temp;
            }
            _buckets[i] = nullptr;
        }
        _size = 0;
    }

    void insert(const K& key, const V& value) {
        if (static_cast<float>(_size) / _capacity >= 1.0) {
            rehash();
        }

        size_t index = get_hash(key) & (_capacity - 1);
        Hash_Node<K, V>* head = _buckets[index];

        while (head) {
            if (head->key == key) {
                head->value = value;
                return;
            }
            head = head->next;
        }

        Hash_Node<K, V>* new_node = new Hash_Node<K, V>(key, value);
        new_node->next = _buckets[index];
        _buckets[index] = new_node;
        ++_size;
    }

    V& at(const K& key) {
        size_t index = get_hash(key) & (_capacity - 1);
        Hash_Node<K, V>* head = _buckets[index];

        while (head) {
            if (head->key == key) {
                return head->value;
            }
            head = head->next;
        }

        throw std::out_of_range("Key not found");
    }

    const V& at(const K& key) const {
        size_t index = get_hash(key) & (_capacity - 1);
        Hash_Node<K, V>* head = _buckets[index];

        while (head) {
            if (head->key == key) {
                return head->value;
            }
            head = head->next;
        }

        throw std::out_of_range("Key not found");
    }
    
    size_t count(const K& key) const {
        size_t index = get_hash(key) & (_capacity - 1);
        Hash_Node<K, V>* head = _buckets[index];

        while (head) {
            if (head->key == key) {
                return 1;
            }
            head = head->next;
        }

        return 0;
    }

    iterator find(const K& key) {
        size_t index = get_hash(key) & (_capacity - 1);
        Hash_Node<K, V>* head = _buckets[index];

        while (head) {
            if (head->key == key) {
                return iterator(head, this, index);
            }
            head = head->next;
        }

        return end();
    }

    const_iterator find(const K& key) const {
        size_t index = get_hash(key) & (_capacity - 1);
        Hash_Node<K, V>* head = _buckets[index];

        while (head) {
            if (head->key == key) {
                return const_iterator(head, this, index);
            }
            head = head->next;
        }

        return end();
    }

    bool contains(const K& key) const {
        return count(key) > 0;
    }

    V& operator[](const K& key) {
        if (static_cast<float>(_size) / _capacity >= 1.0) {
            rehash();
        }

        size_t index = get_hash(key) & (_capacity - 1);
        Hash_Node<K, V>* head = _buckets[index];

        while (head) {
            if (head->key == key) {
                return head->value;
            }
            head = head->next;
        }

        Hash_Node<K, V>* new_node = new Hash_Node<K, V>(key, V());
        new_node->next = _buckets[index];
        _buckets[index] = new_node;
        ++_size;
        return new_node->value;
    }

    size_t erase(const K& key) {
        size_t index = get_hash(key) & (_capacity - 1);
        Hash_Node<K, V>* head = _buckets[index];
        Hash_Node<K, V>* prev = nullptr;

        while (head) {
            if (head->key == key) {
                if (prev == nullptr) {
                    _buckets[index] = head->next;
                } else {
                    prev->next = head->next;
                }
                delete head;
                --_size;
                return 1;
            }
            prev = head;
            head = head->next;
        }

        return 0;
    }

private:
    std::vector<Hash_Node<K, V>*> _buckets;
    size_t _size;
    size_t _capacity;

    size_t get_hash(const K& key) const {
        return std::hash<K>{}(key);
    }

    void rehash() {
        _capacity *= 2;
        std::vector<Hash_Node<K, V>*> new_buckets(_capacity, nullptr);

        for (auto head : _buckets) {
            Hash_Node<K, V>* curr = head;
            while (curr != nullptr) {
                Hash_Node<K, V>* next = curr->next;
                size_t new_index = get_hash(curr->key) & (_capacity - 1);

                curr->next = new_buckets[new_index];
                new_buckets[new_index] = curr;

                curr = next;
            }
        }

        _buckets = std::move(new_buckets);
    }

    size_t round_power_of_two(size_t n) {
        if (n == 0) return 1;
        if ((n & (n - 1)) == 0) return n;
        size_t power = 1;
        while (power < n) {
            power <<= 1;
        }
        return power;
    }
};
