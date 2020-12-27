#pragma once

#include <algorithm>

class ListHook {
public:
    ListHook() {
    }

    bool IsLinked() const {
        return this->prev_ != nullptr || this->next_ != nullptr;
    }

    void Unlink() {
        this->prev_->next_ = this->next_;
        this->next_->prev_ = this->prev_;
        this->next_ = nullptr;
        this->prev_ = nullptr;
    }

    // Must unlink element from list
    ~ListHook() {
        if (IsLinked()) {
            Unlink();
        }
    }

    ListHook(const ListHook&) = delete;

private:
    template <class T>
    friend class List;
    ListHook* prev_ = nullptr;
    ListHook* next_ = nullptr;
    // that helper function might be useful
    void LinkAfter(ListHook* other) {
        other->next_ = this->next_;
        other->prev_ = this;
        this->next_->prev_ = other;
        this->next_ = other;
    }
};

template <typename T>
class List {
public:
    class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    public:
        Iterator(ListHook* mover) : mover_(mover) {
        }
        Iterator& operator++() {
            mover_ = mover_->next_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator temp = *this;
            mover_ = mover_->next_;
            return temp;
        }

        T& operator*() const {
            return *static_cast<T*>(mover_);
        }
        T* operator->() const {
            return static_cast<T*>(mover_);
        }

        bool operator==(const Iterator& rhs) const {
            return mover_ == rhs.mover_;
        }
        bool operator!=(const Iterator& rhs) const {
            return mover_ == rhs.mover_;
        }

    private:
        ListHook* mover_;
    };

    List() : dummy_(new ListHook) {
        dummy_->next_ = dummy_;
        dummy_->prev_ = dummy_;
    }
    List(const List&) = delete;
    List(List&& other) : List() {
        std::swap(dummy_, other.dummy_);
    }

    // must unlink all elements from list
    ~List() {
        while (!IsEmpty()) {
            PopBack();
        }
        delete dummy_;
    }

    List& operator=(const List&) = delete;
    List& operator=(List&& other) {
        if (this == &other) {
            return *this;
        }
        std::swap(dummy_, other.dummy_);
        return *this;
    }

    bool IsEmpty() const {
        return dummy_->next_ == dummy_;
    }
    // that method is allowed to be O(n)
    size_t Size() const {
        int size = 0;
        auto cur = dummy_->next_;
        while (cur != dummy_) {
            ++size;
            cur = cur->next_;
        }
        return size;
    }

    // note that IntrusiveList doesn't own elements,
    // and never copies or moves T
    void PushBack(T* elem) {
        dummy_->prev_->LinkAfter(elem);
    }
    void PushFront(T* elem) {
        dummy_->LinkAfter(elem);
    }

    T& Front() {
        return static_cast<T&>(*(dummy_->next_));
    }
    const T& Front() const {
        return static_cast<T&>(*(dummy_->next_));
    }

    T& Back() {
        return static_cast<T&>(*(dummy_->prev_));
    }
    const T& Back() const {
        return static_cast<T&>(*(dummy_->prev_));
    }

    void PopBack() {
        dummy_->prev_->Unlink();
    }
    void PopFront() {
        dummy_->next_->Unlink();
    }

    Iterator Begin() {
        return {dummy_->next_};
    }
    Iterator End() {
        return {dummy_};
    }

    // complexity of this function must be O(1)
    Iterator IteratorTo(T* element) {
        return {element};
    }

private:
    ListHook* dummy_;
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}
