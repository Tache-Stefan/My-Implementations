#pragma once

#include <algorithm>
#include <cstddef>
#include <limits>
#include <stdexcept>

namespace my_std {

    template<typename T, size_t N>
    class Array {
        public:
            using iterator = T*;
            using const_iterator = const T*;

            constexpr Array() = default;

            constexpr inline bool empty() const noexcept { return N == 0; }
            constexpr inline size_t size() const noexcept { return N; }
            constexpr inline size_t max_size() const noexcept { return N; }

            constexpr T& at(const size_t index);
            constexpr const T& at(const size_t index) const;
            constexpr inline T& operator[](const size_t index) noexcept { return m_data[index]; }
            constexpr inline const T& operator[](const size_t index) const noexcept { return m_data[index]; }
            constexpr inline T& front() noexcept { return m_data[0]; }
            constexpr inline const T& front() const noexcept { return m_data[0]; }
            constexpr inline T& back() noexcept { return m_data[N - 1]; }
            constexpr inline const T& back() const noexcept { return m_data[N - 1]; }
            constexpr inline T* data() noexcept { return m_data; }
            constexpr inline const T* data() const noexcept { return m_data; }

            constexpr inline iterator begin() noexcept { return m_data; }
            constexpr inline iterator end() noexcept { return m_data + N; }
            constexpr inline const_iterator begin() const noexcept { return m_data; }
            constexpr inline const_iterator end() const noexcept { return m_data + N; }
            constexpr inline const_iterator cbegin() const noexcept { return m_data; }
            constexpr inline const_iterator cend() const noexcept { return m_data + N; }

            constexpr void fill(const T& value);
            constexpr void swap(Array& other);

        private:
            T m_data[N > 0 ? N : 1];
    };

    template<typename T, size_t N>
    constexpr T& Array<T, N>::at(const size_t index) {
        if (index >= N) throw std::out_of_range("Array::at");
        return m_data[index];
    }

    template<typename T, size_t N>
    constexpr const T& Array<T, N>::at(const size_t index) const {
        if (index >= N) throw std::out_of_range("Array::at");
        return m_data[index];
    }

    template<typename T, size_t N>
    constexpr void Array<T, N>::fill(const T& value) {
        for (size_t i = 0; i < N; ++i) {
            m_data[i] = value;
        }
    }

    template<typename T, size_t N>
    constexpr void Array<T, N>::swap(Array& other) {
        for (size_t i = 0; i < N; ++i) {
            std::swap(m_data[i], other.m_data[i]);
        }
    }

}
