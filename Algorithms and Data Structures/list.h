#pragma once

template <typename T>
class List {
public:
    struct ListNode {
        ListNode(const T& elem) {
            value_ = elem;
        }
        ListNode(T&& elem) : value_(std::move(elem)) {
        }
        ListNode(ListNode&& node) {
            next_ = std::move(node.next_);
            prev_ = std::move(node.prev_);
            value_ = std::move(node.value_);
        }
        ListNode& operator=(const ListNode& node) {
            if (this == &node) {
                return *this;
            }
            this->~ListNode();
            new (this) ListNode(node);
            return *this;
        }
        ListNode& operator=(ListNode&& node) {
            if (this == &node) {
                return *this;
            }
            this->~ListNode();
            new (this) ListNode(node);
            return *this;
        }

        ~ListNode() {
            if (next_) {
                delete next_;
            }
            if (prev_) {
                delete prev_;
            }
        }

        ListNode* next_ = nullptr;
        ListNode* prev_ = nullptr;
        T value_;
    };
    class Iterator {
    public:
        Iterator(List<T>& list, bool flag = false)
            : head_(list.head_), end_(list.tail_), mover_(list.head_) {
            if (flag) {
                mover_ = nullptr;
            }
        }
        Iterator& operator++() {
            if (!mover_) {
                mover_ = head_;
                return *this;
            }
            mover_ = mover_->next_;
            return *this;
        }
        Iterator operator++(int) {
            if (!mover_) {
                mover_ = head_;
                return *this;
            }
            auto tmp = *this;
            mover_ = mover_->next_;
            return tmp;
        }

        Iterator& operator--() {
            if (!mover_) {
                mover_ = end_;
                return *this;
            }
            mover_ = mover_->prev_;
            return *this;
        }
        Iterator operator--(int) {
            if (!mover_) {
                mover_ = end_;
                return *this;
            }
            auto tmp = *this;
            mover_ = mover_->prev_;
            return tmp;
        }

        T& operator*() const {
            return mover_->value_;
        }
        T* operator->() const {
            return mover_->value_;
        }

        bool operator==(const Iterator& rhs) const {
            return mover_ == rhs.mover_;
        }
        bool operator!=(const Iterator& rhs) const {
            return mover_ != rhs.mover_;
        }

    private:
        ListNode* head_;
        ListNode* end_;
        ListNode* mover_;
    };

    List() {
    }
    List(const List& list) {
        auto tmp = list.head_;
        while (tmp) {
            PushBack(tmp->value_);
            tmp = tmp->next_;
        }
    }
    List(List&& list) {
        head_ = list.head_;
        list.head_ = nullptr;
        tail_ = list.tail_;
        list.tail_ = nullptr;
        size_ = list.size_;
        list.size_ = 0;
    }
    ~List() {
        while (head_) {
            Unlink(head_);
        }
    }

    List& operator=(const List& list) {
        if (this == &list) {
            return *this;
        }
        this->~List();
        new (this) List(list);
        return *this;
    }
    List& operator=(List&& list) {
        if (this == &list) {
            return *this;
        }
        this->~List();
        new (this) List(std::move(list));
        return *this;
    }

    bool IsEmpty() const {
        return size_ == 0;
    }
    size_t Size() const {
        return size_;
    }

    void PushBack(const T& elem) {
        LinkAfter(new ListNode(elem), tail_);
    }
    void PushBack(T&& elem) {
        LinkAfter(new ListNode(std::move(elem)), tail_);
    }
    void PushFront(const T& elem) {
        LinkAfter(new ListNode(elem), nullptr);
    }
    void PushFront(T&& elem) {
        LinkAfter(new ListNode(elem), nullptr);
    }

    T& Front() {
        if (head_) {
            return head_->value_;
        }
    }
    const T& Front() const {
        if (head_) {
            return head_->value_;
        }
    }
    T& Back() {
        if (tail_) {
            return tail_->value_;
        }
    }
    const T& Back() const {
        if (tail_) {
            return tail_->value_;
        }
    }

    void PopBack() {
        Unlink(tail_);
    }
    void PopFront() {
        Unlink(head_);
    }

    Iterator Begin() {
        return Iterator(*this);
    }
    Iterator End() {
        return Iterator(*this, true);
    }

private:
    void Unlink(ListNode* node) {
        if (node) {
            if (!node->prev_ && !node->next_) {
                head_ = tail_ = nullptr;
                --size_;
                delete node;
                return;
            }
            if (!node->prev_) {
                head_ = node->next_;
                node->prev_ = nullptr;
                node->next_ = nullptr;
                delete node;
                head_->prev_ = nullptr;
                --size_;
                return;
            }
            if (!node->next_) {
                tail_ = node->prev_;
                node->prev_ = nullptr;
                node->next_ = nullptr;
                delete node;
                tail_->next_ = nullptr;
                --size_;
                return;
            }
        }
    }
    void LinkAfter(ListNode* target, ListNode* after) {
        if (!head_) {
            head_ = tail_ = target;
            ++size_;
            return;
        }
        if (!after) {
            target->next_ = head_;
            ++size_;
            head_->prev_ = target;
            head_ = target;
            return;
        }
        if (after == tail_) {
            tail_->next_ = target;
            target->prev_ = tail_;
            tail_ = target;
            ++size_;
        }
    }
    ListNode* head_ = nullptr;
    ListNode* tail_ = nullptr;
    int size_ = 0;
    friend class Iterator;
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}