#pragma once

#include <algorithm>
#include <cstddef>
#include <limits>
#include <stdexcept>

namespace my_std {

    template<typename T>
    class Vector {
        public:
            Vector() = default;

            void push_back(const T& value);
            void pop_back();

            inline void clear() { m_size = 0; } 
            void reserve(const size_t new_cap);

            inline bool empty() const { return m_size == 0; }
            inline size_t size() const { return m_size; }
            inline size_t capacity() const { return m_capacity; }
            inline size_t max_size() const { return std::numeric_limits<size_t>::max(); }

            T& operator[](const size_t index) { return m_data[index]; }
            const T& operator[](const size_t index) const { return m_data[index]; }
            T& at(const size_t index);
            const T& at(const size_t index) const;

            Vector(const Vector& other);
            Vector& operator=(const Vector& other);

            Vector(Vector&& other) noexcept;
            Vector& operator=(Vector&& other) noexcept;

            ~Vector() { delete[] m_data; }

        private:
            T* m_data{ nullptr };
            size_t m_size{ 0 };
            size_t m_capacity{ 0 };
            static constexpr size_t m_INIT_CAP = 10;
    };

    template<typename T>
    void Vector<T>::push_back(const T& value) {
        if (m_size >= m_capacity) {
            reserve(m_capacity == 0 ? m_INIT_CAP : m_capacity * 2);
        }

        m_data[m_size++] = value;
    }

    template<typename T>
    void Vector<T>::pop_back() {
        if (m_size > 0) --m_size;
    }

    template<typename T>
    void Vector<T>::reserve(const size_t new_cap) {
        if (new_cap <= m_capacity) return;

        T* new_data = new T[new_cap];
        std::copy(m_data, m_data + m_size, new_data);
        delete[] m_data;
        m_data = new_data;
        m_capacity = new_cap;
    }

    template<typename T>
    T& Vector<T>::at(const size_t index) {
        if (index >= m_size) throw std::out_of_range("Vector::at");
        return m_data[index];
    }

    template<typename T>
    const T& Vector<T>::at(const size_t index) const {
        if (index >= m_size) throw std::out_of_range("Vector::at");
        return m_data[index];
    }

    template<typename T>
    Vector<T>::Vector(const Vector& other)
        : m_data(other.m_capacity ? new T[other.m_capacity] : nullptr),
          m_size(other.m_size),
          m_capacity(other.m_capacity) {
        std::copy(other.m_data, other.m_data + m_size, m_data);
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(const Vector& other) {
        if (this == &other) return *this;

        delete[] m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_data = m_capacity ? new T[m_capacity] : nullptr;
        std::copy(other.m_data, other.m_data + m_size, m_data);
        return *this;
    }

    template<typename T>
    Vector<T>::Vector(Vector&& other) noexcept
        : m_data(other.m_data),
          m_size(other.m_size),
          m_capacity(other.m_capacity) {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
        if (this == &other) return *this;

        delete[] m_data;
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
        return *this;
    }

}
