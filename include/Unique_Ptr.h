#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace my_std {

    template<typename T>
    class Unique_Ptr {
    private:
        T* _ptr;

    public:
        explicit Unique_Ptr(T *ptr = nullptr) : _ptr(ptr) {}

        Unique_Ptr(const Unique_Ptr&) = delete;
        Unique_Ptr& operator=(const Unique_Ptr&) = delete;

        Unique_Ptr(Unique_Ptr&& other) noexcept;
        Unique_Ptr& operator=(Unique_Ptr&& other) noexcept;

        ~Unique_Ptr() {
            delete _ptr;
        }

        T* release() noexcept;
        void reset(T *ptr = nullptr) noexcept;

        explicit operator bool() const noexcept { return _ptr != nullptr; }
        T& operator*() const { return *_ptr; }
        T* operator->() const { return _ptr; }
        T* get() const { return _ptr; }
    };

    template<typename T>
    Unique_Ptr<T>::Unique_Ptr(Unique_Ptr&& other) noexcept : _ptr(other._ptr) {
        other._ptr = nullptr;
    }

    template<typename T>
    Unique_Ptr<T>& Unique_Ptr<T>::operator=(Unique_Ptr<T>&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete _ptr;
        _ptr = other._ptr;
        other._ptr = nullptr;
        return *this;
    }

    template<typename T>
    T* Unique_Ptr<T>::release() noexcept {
        T *temp = _ptr;
        _ptr = nullptr;
        return temp;
    }

    template<typename T>
    void Unique_Ptr<T>::reset(T *ptr) noexcept {
        if (ptr == _ptr) return;
        T *old_ptr = _ptr;
        _ptr = ptr;
        if (old_ptr) {
            delete old_ptr;
        }
    }

    template<typename T>
    class Unique_Ptr<T[]> {
    private:
        T* _ptr;

    public:
        explicit Unique_Ptr(T *ptr = nullptr) : _ptr(ptr) {}

        Unique_Ptr(const Unique_Ptr&) = delete;
        Unique_Ptr& operator=(const Unique_Ptr&) = delete;

        Unique_Ptr(Unique_Ptr&& other) noexcept;
        Unique_Ptr& operator=(Unique_Ptr&& other) noexcept;

        ~Unique_Ptr() {
            delete[] _ptr;
        }

        explicit operator bool() const noexcept { return _ptr != nullptr; }
        T& operator[](std::size_t i) const { return _ptr[i]; }

        T* release() noexcept;
        void reset(T *ptr = nullptr) noexcept;

        T* get() const { return _ptr; }
    };

    template<typename T>
    Unique_Ptr<T[]>::Unique_Ptr(Unique_Ptr&& other) noexcept : _ptr(other._ptr) {
        other._ptr = nullptr;
    }

    template<typename T>
    Unique_Ptr<T[]>& Unique_Ptr<T[]>::operator=(Unique_Ptr<T[]>&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete[] _ptr;
        _ptr = other._ptr;
        other._ptr = nullptr;
        return *this;
    }

    template<typename T>
    T* Unique_Ptr<T[]>::release() noexcept {
        T *temp = _ptr;
        _ptr = nullptr;
        return temp;
    }

    template<typename T>
    void Unique_Ptr<T[]>::reset(T *ptr) noexcept {
        if (ptr == _ptr) return;
        T *old_ptr = _ptr;
        _ptr = ptr;
        if (old_ptr) {
            delete[] old_ptr;
        }
    }

    template<typename T, typename... Args>
    typename std::enable_if_t<!std::is_array_v<T>, Unique_Ptr<T>>
    make_unique(Args&&... args) {
        return Unique_Ptr<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T>
    typename std::enable_if_t<std::is_array_v<T>, Unique_Ptr<T>>
    make_unique(std::size_t size) {
        return Unique_Ptr<T>(new std::remove_extent_t<T>[size]());
    }

}
