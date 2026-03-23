#pragma once

#include <cstddef>
#include <utility>

namespace my_std {

    template<typename T, size_t N>
    class Ring_Buffer {
    public:
        Ring_Buffer() = default;

        void try_push(const T& value);
        void try_push(T&& value);
        void push(const T& value);
        void push(T&& value);
        void pop() noexcept;

        inline bool empty() const noexcept { return m_size == 0; }
        inline bool full() const noexcept { return m_size == N; }
        inline size_t size() const noexcept { return m_size; }
        inline const T& front() const noexcept { return m_data[m_front]; }
        inline const T& back() const noexcept { return m_data[(m_back + N - 1) % N]; }

    private:
        T m_data[N];
        size_t m_size = 0;
        size_t m_front = 0;
        size_t m_back = 0;
    };

    template<typename T, size_t N>
    void Ring_Buffer<T, N>::try_push(const T& value) {
        if (full()) return;

        m_data[m_back] = value;
        m_back = (m_back + 1) % N;
        ++m_size;
    }

    template<typename T, size_t N>
    void Ring_Buffer<T, N>::try_push(T&& value) {
        if (full()) return;

        m_data[m_back] = std::move(value);
        m_back = (m_back + 1) % N;
        ++m_size;
    }

    template<typename T, size_t N>
    void Ring_Buffer<T, N>::push(const T& value) {
        if (full()) {
            m_front = (m_front + 1) % N;
        } else {
            ++m_size;
        }

        m_data[m_back] = value;
        m_back = (m_back + 1) % N;
    }

    template<typename T, size_t N>
    void Ring_Buffer<T, N>::push(T&& value) {
        if (full()) {
            m_front = (m_front + 1) % N;
        } else {
            ++m_size;
        }

        m_data[m_back] = std::move(value);
        m_back = (m_back + 1) % N;
    }

    template<typename T, size_t N>
    void Ring_Buffer<T, N>::pop() noexcept {
        if (empty()) return;
        m_front = (m_front + 1) % N;
        --m_size;
    }

}
