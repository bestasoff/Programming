#pragma once

class Vector {
public:
    Vector() : vector_(new int[0]), size_(0), capacity_(0) {
    }

    Vector(int size) : vector_{new int[size]}, size_(5), capacity_(size) {
        for (int i = 0; i < capacity_; ++i) {
            vector_[i] = 0;
        }
    }

    Vector(std::initializer_list<int> list)
        : vector_(new int[list.size()]), size_(list.size()), capacity_(list.size()) {
        std::copy(list.begin(), list.end(), vector_);
    }

    Vector(const Vector& vector)
        : vector_(new int[vector.capacity_]), size_(vector.size_), capacity_(vector.capacity_) {
        std::copy(vector.vector_, vector.vector_ + size_, vector_);
    }

    Vector(Vector&& vector)
        : vector_(vector.vector_), size_(vector.size_), capacity_(vector.capacity_) {
        vector.vector_ = nullptr;
        vector.size_ = vector.capacity_ = 0;
    }

    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }

        this->~Vector();
        new (this) Vector(other);
        return *this;
    }

    Vector& operator=(Vector&& other) {
        if (this == &other) {
            return *this;
        }

        this->~Vector();
        new (this) Vector(other);
        return *this;
    }

    void Swap(Vector& vector) {
        std::swap(size_, vector.size_);
        std::swap(capacity_, vector.capacity_);
        std::swap(vector_, vector.vector_);
    }

    int& operator[](int index) const {
        return vector_[index];
    }

    int& operator[](int index) {
        return vector_[index];
    }

    int Size() const {
        return size_;
    }

    int Capacity() const {
        return capacity_;
    }

    void PushBack(int element) {
        if (size_ == capacity_) {
            Reallocate(capacity_ == 0 ? 1 : 2 * capacity_);
        }
        vector_[size_++] = element;
    }

    void PopBack() {
        --size_;
    }

    void Clear() {
        size_ = 0;
    }

    void Reserve(int capacity) {
        if (capacity <= capacity_) {
            return;
        }

        Reallocate(capacity);
    }

    class Iterator : public std::iterator<std::random_access_iterator_tag, int, int, int*, int> {
    public:
        explicit Iterator() : vector_(nullptr) {
        }
        explicit Iterator(int* vec) : vector_(vec) {
        }

        Iterator& operator+=(int step) {
            vector_ += step;
            return *this;
        }

        Iterator& operator-=(int step) {
            vector_ -= step;
            return *this;
        }

        Iterator& operator++() {
            vector_ += 1;
            return *this;
        }

        Iterator operator++(int) {
            auto tmp = *this;
            vector_++;
            return tmp;
        }

        Iterator& operator--() {
            --vector_;
            return *this;
        }

        Iterator operator--(int) {
            auto tmp = *this;
            vector_--;
            return tmp;
        }

        Iterator operator+(int step) {
            return Iterator(vector_ + step);
        }

        Iterator operator-(int step) {
            return Iterator(vector_ - step);
        }

        int operator-(Iterator& other) {
            return vector_ - other.vector_;
        }

        bool operator==(Iterator other) const {
            return vector_ == other.vector_;
        }

        bool operator!=(Iterator& other) const {
            return vector_ != other.vector_;
        }

        int operator*() {
            return *vector_;
        }

        int* operator->() {
            return vector_;
        }

    private:
        int* vector_;
    };

    Iterator begin() {  //  NOLINT
        return Iterator(vector_);
    }

    Iterator end() {  // NOLINT
        return Iterator(vector_ + size_);
    }

    Iterator Begin() {
        return Iterator(vector_);
    }

    Iterator End() {
        return Iterator(vector_ + size_);
    }

    ~Vector() {
        delete[] vector_;
    }

private:
    void Reallocate(int capacity) {
        int* tmp = new int[capacity];
        capacity_ = capacity;
        std::copy(vector_, vector_ + size_, tmp);
        delete[] vector_;
        vector_ = tmp;
    }

    int* vector_;
    int size_;
    int capacity_;
};
