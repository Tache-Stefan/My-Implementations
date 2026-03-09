#pragma once

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <utility>

namespace my_std {

    template<typename T>
    class Vector {
        public:
            using iterator = T*;
            using const_iterator = const T*;

            Vector() = default;
            Vector(std::initializer_list<T> init);

            void push_back(const T& value);
            void push_back(T&& value);
            template<typename... Args>
            void emplace_back(Args&&... args);
            void pop_back() noexcept;

            inline void clear() noexcept { m_size = 0; } 
            void reserve(const size_t new_cap);

            inline bool empty() const noexcept { return m_size == 0; }
            inline size_t size() const  noexcept { return m_size; }
            inline size_t capacity() const noexcept { return m_capacity; }
            inline size_t max_size() const noexcept { return std::numeric_limits<size_t>::max(); }

            inline iterator begin() noexcept { return m_data; }
            inline iterator end() noexcept { return m_data + m_size; }
            inline const_iterator begin() const noexcept { return m_data; }
            inline const_iterator end() const noexcept { return m_data + m_size; }
            inline const_iterator cbegin() const noexcept { return m_data; }
            inline const_iterator cend() const noexcept { return m_data + m_size; }

            T& operator[](const size_t index) noexcept { return m_data[index]; }
            const T& operator[](const size_t index) const noexcept { return m_data[index]; }
            T& at(const size_t index);
            const T& at(const size_t index) const;
            T& front() noexcept { return m_data[0]; }
            const T& front() const noexcept { return m_data[0]; }
            T& back() noexcept { return m_data[m_size - 1]; }
            const T& back() const noexcept { return m_data[m_size - 1]; }

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
    Vector<T>::Vector(std::initializer_list<T> init)
        : m_data(init.size() ? new T[init.size()] : nullptr),
          m_size(init.size()),
          m_capacity(init.size()) {
        std::copy(init.begin(), init.end(), m_data);
    }

    template<typename T>
    void Vector<T>::push_back(const T& value) {
        if (m_size >= m_capacity) {
            reserve(m_capacity == 0 ? m_INIT_CAP : m_capacity * 2);
        }

        m_data[m_size++] = value;
    }

    template<typename T>
    void Vector<T>::push_back(T&& value) {
        if (m_size >= m_capacity) {
            reserve(m_capacity == 0 ? m_INIT_CAP : m_capacity * 2);
        }

        m_data[m_size++] = std::move(value);
    }

    template<typename T>
    template<typename... Args>
    void Vector<T>::emplace_back(Args&&... args) {
        if (m_size >= m_capacity) {
            reserve(m_capacity == 0 ? m_INIT_CAP : m_capacity * 2);
        }

        m_data[m_size++] = T(std::forward<Args>(args)...);
    }

    template<typename T>
    void Vector<T>::pop_back() noexcept {
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
