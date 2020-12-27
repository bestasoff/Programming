#pragma once

#include <memory>

class Any {
    struct Base {
        virtual ~Base() = default;
        virtual std::unique_ptr<Base> Clone() = 0;
    };

    template <class T>
    struct Derived : public Base {
        T value;

        explicit Derived(T value) : value(std::move(value)) {
        }
        std::unique_ptr<Base> Clone() override {
            return std::make_unique<Derived<T>>(value);
        }
    };

public:
    Any() {
    }

    template <class T>
    Any(const T& value) : ptr_(std::make_unique<Derived<T>>(value)) {
    }

    template <class T>
    Any& operator=(const T& value) {
        this->~Any();
        new (this) Any(value);
        return *this;
    }

    Any(const Any& rhs) {
        if (rhs.ptr_) {
            ptr_ = rhs.ptr_->Clone();
        } else {
            ptr_ = nullptr;
        }
    }
    Any& operator=(const Any& rhs) {
        if (this == &rhs) {
            return *this;
        }
        this->~Any();
        new (this) Any(rhs);
        return *this;
    }
    ~Any() {
    }

    bool Empty() const {
        return ptr_ == nullptr;
    }

    void Clear() {
        ptr_ = nullptr;
    }
    void Swap(Any& rhs) {
        std::swap(ptr_, rhs.ptr_);
    }

    template <class T>
    const T& GetValue() const {
        return dynamic_cast<const Derived<T>&>(*ptr_).value;
    }

private:
    std::unique_ptr<Base> ptr_ = nullptr;
};
