#pragma once

#include <cstddef>
#include <utility>

namespace my_std {

    template<typename T, size_t m_capacity>
    class Ring_Buffer {
        public:
            Ring_Buffer() = default;

            void try_push(const T& value);
            void try_push(T&& value);
            void push(const T& value);
            void push(T&& value);
            void pop() noexcept;

            inline bool empty() const noexcept { return m_size == 0; }
            inline bool full() const noexcept { return m_size == m_capacity; }
            inline size_t size() const noexcept { return m_size; }
            inline const T& front() const noexcept { return m_data[m_front]; }
            inline const T& back() const noexcept { return m_data[(m_back + m_capacity - 1) % m_capacity]; }

        private:
            T m_data[m_capacity];
            size_t m_size = 0;
            size_t m_front = 0;
            size_t m_back = 0;
    };

    template<typename T, size_t m_capacity>
    void Ring_Buffer<T, m_capacity>::try_push(const T& value) {
        if (full()) return;

        m_data[m_back] = value;
        m_back = (m_back + 1) % m_capacity;
        ++m_size;
    }

    template<typename T, size_t m_capacity>
    void Ring_Buffer<T, m_capacity>::try_push(T&& value) {
        if (full()) return;

        m_data[m_back] = std::move(value);
        m_back = (m_back + 1) % m_capacity;
        ++m_size;
    }

    template<typename T, size_t m_capacity>
    void Ring_Buffer<T, m_capacity>::push(const T& value) {
        if (full()) {
            m_front = (m_front + 1) % m_capacity;
        } else {
            ++m_size;
        }

        m_data[m_back] = value;
        m_back = (m_back + 1) % m_capacity;
    }

    template<typename T, size_t m_capacity>
    void Ring_Buffer<T, m_capacity>::push(T&& value) {
        if (full()) {
            m_front = (m_front + 1) % m_capacity;
        } else {
            ++m_size;
        }

        m_data[m_back] = std::move(value);
        m_back = (m_back + 1) % m_capacity;
    }

    template<typename T, size_t m_capacity>
    void Ring_Buffer<T, m_capacity>::pop() noexcept {
        if (empty()) return;
        m_front = (m_front + 1) % m_capacity;
        --m_size;
    }

}
