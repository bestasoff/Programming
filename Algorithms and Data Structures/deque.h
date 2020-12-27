#pragma once

#include <initializer_list>
#include <algorithm>

class Deque {
public:
    Deque() {
    }
    Deque(const Deque& rhs) {
        if (this == &rhs) {
            return;
        }
        for (int i = 0; i < rhs.size_; ++i) {
            PushBack(rhs[i]);
        }
    }
    Deque(Deque&& rhs) : size_(rhs.size_) {
        if (this == &rhs) {
            return;
        }
        data_ = rhs.data_;
        rhs.size_ = 0;
        rhs.data_ = nullptr;
        block_count_ = rhs.block_count_;
        end_block_ = rhs.end_block_;
        end_pos_ = rhs.end_pos_;
        begin_pos_ = rhs.begin_pos_;
        begin_block_ = rhs.begin_block_;
    }
    explicit Deque(int size) {
        for (int i = 0; i < size; ++i) {
            PushBack(0);
        }
    }

    Deque(std::initializer_list<int> list) {
        for (auto el : list) {
            PushBack(el);
        }
    }

    Deque& operator=(Deque rhs) {
        Swap(rhs);
        return *this;
    }

    void Swap(Deque& rhs) {
        std::swap(data_, rhs.data_);
        std::swap(block_count_, rhs.block_count_);
        std::swap(begin_block_, rhs.begin_block_);
        std::swap(begin_pos_, rhs.begin_pos_);
        std::swap(end_block_, rhs.end_block_);
        std::swap(end_pos_, rhs.end_pos_);
        std::swap(size_, rhs.size_);
    }

    void PushBack(int value) {
        if (!block_count_) {
            Reallocate(2);
            end_block_ = 1;
            begin_pos_ = block_size_ - 1;
        }
        data_[end_block_][end_pos_] = value;
        if (end_pos_ == block_size_ - 1) {
            if (end_block_ == block_count_ - 1) {
                Reallocate(block_count_ * 2);
            }
            ++end_block_;
            end_pos_ = 0;
        } else {
            ++end_pos_;
        }
        ++size_;
    }

    void PopBack() {
        if (end_pos_ == 0) {
            end_pos_ = block_size_;
            --end_block_;
        }
        --end_pos_;
        --size_;
    }

    void PushFront(int value) {
        if (!block_count_) {
            Reallocate(2);
            begin_pos_ = block_size_ - 1;
            end_block_ = 1;
        }

        data_[begin_block_][begin_pos_] = value;
        if (begin_pos_ == 0) {
            if (begin_block_ == 0) {
                Reallocate(2 * block_count_);
            }
            --begin_block_;
            begin_pos_ = block_size_ - 1;
        } else {
            --begin_pos_;
        }
        ++size_;
    }

    void PopFront() {
        if (begin_pos_ == block_size_ - 1) {
            begin_pos_ = 0;
            ++begin_block_;
            --size_;
        } else {
            ++begin_pos_;
            --size_;
        }
    }

    std::pair<int, int> CountIndex(int ind) const {
        if (begin_block_ == end_block_) {
            return {begin_block_, begin_pos_ + ind + 1};
        }
        if (ind < block_size_ - begin_pos_ - 1) {
            return {begin_block_, begin_pos_ + 1 + ind};
        }
        ind -= block_size_ - begin_pos_ - 1;
        int cur_block = ind / block_size_ + 1;
        return {begin_block_ + cur_block, ind % block_size_};
    }

    int& operator[](size_t ind) {
        auto index = CountIndex(ind);
        return data_[index.first][index.second];
    }

    int operator[](size_t ind) const {
        auto index = CountIndex(ind);
        return data_[index.first][index.second];
    }

    size_t Size() const {
        return size_;
    }

    void Clear() {
        if (data_) {
            for (int i = 0; i < block_count_; ++i) {
                delete[] data_[i];
            }
            delete[] data_;
        }
        data_ = new int*[1];
        block_count_ = 0;
        size_ = 0;
        begin_block_ = 0;
        begin_pos_ = 0;
        end_block_ = 0;
        end_pos_ = 0;
    }

    ~Deque() {
        if (data_) {
            for (int i = 0; i < block_count_; ++i) {
                delete[] data_[i];
            }
            delete[] data_;
        }
    }

private:
    void Reallocate(int new_block_count) {
        int** new_data = new int*[new_block_count];
        for (int i = 0; i < new_block_count; ++i) {
            new_data[i] = new int[block_size_];
        }
        for (int i = block_count_ / 2; i < block_count_ / 2 + block_count_; ++i) {
            delete[] new_data[i];
            new_data[i] = std::move(data_[i - block_count_ / 2]);
        }
        delete[] data_;
        this->data_ = new_data;
        begin_block_ = block_count_ / 2 + begin_block_;
        end_block_ = block_count_ / 2 + end_block_;
        block_count_ = new_block_count;
    }

    const int block_size_ = 128;
    int begin_pos_ = 0;
    int end_pos_ = 0;
    int begin_block_ = 0;
    int end_block_ = 0;
    int block_count_ = 0;
    int size_ = 0;
    int** data_ = nullptr;
};
