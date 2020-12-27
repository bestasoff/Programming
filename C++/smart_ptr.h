#pragma once

#include <string>

class WeakPtr;

struct Block {
    int strong_counter = 0;
    int weak_counter = 0;
};

class SharedPtr {
public:
    SharedPtr() {
    }
    SharedPtr(std::string* obj) : obj_(obj), counter_(new Block{1, 0}) {
    }
    SharedPtr(const SharedPtr& rhs) : obj_(rhs.obj_), counter_(rhs.counter_) {
        if (counter_) {
            ++counter_->strong_counter;
        }
    }
    SharedPtr(SharedPtr&& rhs) {
        obj_ = rhs.obj_;
        counter_ = rhs.counter_;
        rhs.counter_ = nullptr;
        rhs.obj_ = nullptr;
    }

    void Swap(SharedPtr& rhs) {
        std::swap(this->counter_, rhs.counter_);
        std::swap(this->obj_, rhs.obj_);
    }

    SharedPtr& operator=(SharedPtr rhs) {
        Swap(rhs);
        return *this;
    }

    std::string* Get() const {
        return obj_;
    }

    std::string& operator*() const {
        return *obj_;
    }

    std::string* operator->() const {
        return obj_;
    }

    void Reset(std::string* rhs) {
        this->~SharedPtr();
        new (this) SharedPtr(rhs);
    }

    ~SharedPtr() {
        if (!counter_) {
            return;
        }
        --counter_->strong_counter;
        //        if (counter_->strong_counter == 0) {
        //            delete obj_;
        //        }

        if (counter_->strong_counter + counter_->weak_counter == 0) {
            delete counter_;
            delete obj_;
        }
    }

    SharedPtr(const WeakPtr& rhs);

private:
    std::string* obj_ = nullptr;
    Block* counter_ = nullptr;
    friend class WeakPtr;
};

class WeakPtr {
public:
    WeakPtr() {
    }
    WeakPtr(const WeakPtr& rhs) : obj_(rhs.obj_), counter_(rhs.counter_) {
        if (counter_) {
            ++counter_->weak_counter;
        }
    }
    WeakPtr(const SharedPtr& rhs) : obj_(rhs.obj_), counter_(rhs.counter_) {
        if (counter_) {
            ++counter_->weak_counter;
        }
    }

    void Swap(WeakPtr& rhs) {
        std::swap(this->counter_, rhs.counter_);
        std::swap(this->obj_, rhs.obj_);
    }

    WeakPtr& operator=(WeakPtr rhs) {
        Swap(rhs);
        return *this;
    }

    bool IsExpired() const {
        return !counter_ || (counter_ && counter_->strong_counter == 0);
    }
    SharedPtr Lock() const {
        if (IsExpired()) {
            return SharedPtr();
        }
        return SharedPtr(*this);
    }

    ~WeakPtr() {
        if (!counter_) {
            return;
        }
        --counter_->weak_counter;
        if (counter_->strong_counter + counter_->weak_counter == 0) {
            delete counter_;
            delete obj_;
        }
    }

private:
    std::string* obj_ = nullptr;
    Block* counter_ = nullptr;
    friend class SharedPtr;
};

SharedPtr::SharedPtr(const WeakPtr& rhs) : obj_(rhs.obj_), counter_(rhs.counter_) {
    if (counter_) {
        if (counter_->strong_counter == 0) {
            obj_ = nullptr;
        }
        ++counter_->strong_counter;
    }
}