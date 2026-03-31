#pragma once

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
        T *old_ptr = _ptr;
        _ptr = ptr;
        if (old_ptr) {
            delete old_ptr;
        }
    }

}
