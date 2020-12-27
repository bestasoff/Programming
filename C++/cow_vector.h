#pragma once
#include <vector>
#include <string>
#include <iostream>

struct State {
    State(const std::vector<std::string>& vector) : vector_(vector.begin(), vector.end()) {
        ref_count_ += 1;
    }

    State(const State& state) : ref_count_(state.ref_count_), vector_(state.vector_) {
    }

    int ref_count_ = 0;
    std::vector<std::string> vector_;
};

class COWVector {
public:
    COWVector() : state_(new State({})) {
    }

    ~COWVector() {
        if (state_->ref_count_ == 1) {
            delete state_;
        } else {
            --state_->ref_count_;
        }
    }

    COWVector(const COWVector& other) {
        if (state_ == other.state_) {
            return;
        }

        state_ = other.state_;
        state_->ref_count_++;
    }
    COWVector& operator=(const COWVector& other) {
        if (state_ == other.state_) {
            return *this;
        }

        this->~COWVector();
        state_ = other.state_;
        state_->ref_count_++;
        return *this;
    }

    size_t Size() const {
        return state_->vector_.size();
    }

    void Resize(size_t size) {
        CheckState();
        std::vector<std::string> new_vector(size);
        for (size_t i = 0; i < std::min(size, state_->vector_.size()); i += 1) {
            new_vector[i] = state_->vector_[i];
        }
        if (size > state_->vector_.size()) {
            for (size_t i = std::min(size, state_->vector_.size());
                 i < std::max(size, state_->vector_.size()); i += 1) {
                new_vector[i] = "";
            }
        }
        state_->vector_.clear();
        state_->vector_ = new_vector;
    }

    const std::string& Get(size_t at) {
        return state_->vector_[at];
    }
    const std::string& Back() {
        return state_->vector_.back();
    }

    void PushBack(const std::string& value) {
        if (state_->ref_count_ > 1) {
        }
        state_->vector_.push_back(value);
    }

    void Set(size_t at, const std::string& value) {
        CheckState();
        state_->vector_[at] = value;
    }

private:
    void CheckState() {
        if (state_->ref_count_ > 1) {
            state_->ref_count_ -= 1;
            state_ = new State(state_->vector_);
        }
    }

    State* state_ = nullptr;
};