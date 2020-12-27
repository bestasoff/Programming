#pragma once
#include <utility>

template <class Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {
    }

    Iterator begin() const {  // NOLINT
        return begin_;
    }

    Iterator end() const {  // NOLINT
        return end_;
    }

private:
    Iterator begin_, end_;
};

class RangeIterator {
public:
    RangeIterator(int64_t from, int64_t to, int step) : from_(from), to_(to), step_(step) {
        value_ = from;
    }

    RangeIterator operator++() {
        value_ += step_;
        return *this;
    }

    int64_t& operator*() {
        return value_;
    }

    RangeIterator Begin() {
        return RangeIterator(from_, to_, step_);
    }

    RangeIterator End() {
        return RangeIterator(to_, from_, step_);
    }

    bool operator==(const RangeIterator& rhs) {
        return value_ >= rhs.value_ and value_ > from_;
    }

    bool operator!=(const RangeIterator& rhs) {
        return value_ < rhs.value_ and value_ >= from_;
    }

private:
    int64_t value_;
    int64_t from_;
    int64_t to_;
    int step_;
};

template <class IteratorA, class IteratorB>
class ZipIterator {
public:
    ZipIterator(IteratorA begin_a, IteratorB begin_b) : begin_a_(begin_a), begin_b_(begin_b) {
    }

    ZipIterator operator++() {
        ++begin_a_;
        ++begin_b_;
        return *this;
    }

    auto operator*() {
        return std::make_pair(*begin_a_, *begin_b_);
    }

    bool operator==(const ZipIterator<IteratorA, IteratorB>& rhs) {
        return begin_a_ == rhs.begin_a_ or begin_b_ == rhs.begin_b_;
    }

    bool operator!=(const ZipIterator<IteratorA, IteratorB>& rhs) {
        return begin_a_ != rhs.begin_a_ and begin_b_ != rhs.begin_b_;
    }

private:
    IteratorA begin_a_;
    IteratorB begin_b_;
};

template <class Iterator>
class GroupIterator {
public:
    GroupIterator(Iterator begin, Iterator end) : begin_(begin), end_(end) {
    }

    auto operator*() {
        auto current_val = *begin_;
        auto current_begin = begin_;
        while (current_begin != end_ and *current_begin == current_val) {
            ++current_begin;
        }
        return IteratorRange(begin_, current_begin);
    }

    GroupIterator operator++() {
        auto current_val = *begin_;
        while (begin_ != end_ and *begin_ == current_val) {
            ++begin_;
        }
        return *this;
    }

    bool operator==(const GroupIterator<Iterator>& rhs) {
        return begin_ == rhs.begin_;
    }

    bool operator!=(const GroupIterator<Iterator>& rhs) {
        return begin_ != rhs.begin_;
    }

private:
    Iterator begin_;
    Iterator end_;
};

auto Range(int64_t to) {
    return IteratorRange<RangeIterator>(RangeIterator(0, to, 1).Begin(),
                                        RangeIterator(0, to, 1).End());
}

auto Range(int64_t from, int64_t to) {
    return IteratorRange<RangeIterator>(RangeIterator(from, to, 1).Begin(),
                                        RangeIterator(from, to, 1).End());
}

auto Range(int64_t from, int64_t to, int step) {
    return IteratorRange<RangeIterator>(RangeIterator(from, to, step).Begin(),
                                        RangeIterator(from, to, step).End());
}

template <class VA, class VB>
auto Zip(const VA& a, const VB& b) {
    using Iter = ZipIterator<decltype((static_cast<const VA*>(nullptr))->begin()),
                             decltype((static_cast<const VB*>(nullptr))->begin())>;
    return IteratorRange<Iter>(Iter(a.begin(), b.begin()), Iter(a.end(), b.end()));
}

template <class T>
auto Group(const T& a) {
    using Iter = IteratorRange<GroupIterator<decltype((static_cast<const T*>(nullptr))->begin())>>;
    return Iter(GroupIterator(a.begin(), a.end()), GroupIterator(a.end(), a.end()));
}