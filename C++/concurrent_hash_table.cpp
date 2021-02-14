#include <mutex>
#include <functional>
#include <list>
#include <vector>
#include <atomic>
#include <exception>

template <class K, class V, class Hash = std::hash<K>>
class ConcurrentHashMap {
public:
    ConcurrentHashMap(const Hash& hasher = Hash()) : ConcurrentHashMap(kUndefinedSize, hasher) {
    }

    explicit ConcurrentHashMap(int expected_size, const Hash& hasher = Hash())
        : ConcurrentHashMap(expected_size, kDefaultConcurrencyLevel, hasher) {
    }

    ConcurrentHashMap(int expected_size, int expected_threads_count, const Hash& hasher = Hash())
        : hasher_(hasher) {
        (void)expected_threads_count;
        lock_capacity_ = kDefaultConcurrencyLevel;
        if (expected_size < 8) {
            expected_size = lock_capacity_;
        } else {
            if (expected_size % lock_capacity_ != 0) {
                expected_size += lock_capacity_ - expected_size % lock_capacity_;
            }
        }
        locks_ = std::vector<std::mutex>(lock_capacity_);
        capacity_ = expected_size;
        buckets_.resize(capacity_);
    }

    bool Insert(const K& key, const V& value) {
        size_t lock_idx = GetLock(key);
        std::unique_lock<std::mutex> guard(locks_[lock_idx]);
        size_t idx = GetHash(key);
        for (auto& el : buckets_[idx]) {
            if (el.first == key) {
                return false;
            }
        }
        buckets_[idx].emplace_back(key, value);
        ++size_;
        guard.unlock();
        CheckLoadFactor();
        return true;
    }

    bool Erase(const K& key) {
        size_t lock_idx = GetLock(key);
        std::unique_lock<std::mutex> guard(locks_[lock_idx]);
        size_t idx = GetHash(key);
        for (auto i = buckets_[idx].begin(); i != buckets_[idx].end(); ++i) {
            if ((*i).first == key) {
                buckets_[idx].erase(i);
                --size_;
                return true;
            }
        }
        return false;
    }

    void Clear() {
        for (auto& m : locks_) {
            m.lock();
        }
        buckets_.clear();
        buckets_.resize(lock_capacity_);
        size_ = 0;
        capacity_ = kDefaultConcurrencyLevel;
        for (int i = lock_capacity_ - 1; i >= 0; --i) {
            locks_[i].unlock();
        }
    }

    std::pair<bool, V> Find(const K& key) const {
        size_t lock_idx = GetLock(key);
        std::unique_lock<std::mutex> guard(locks_[lock_idx]);
        size_t idx = GetHash(key);
        for (auto& el : buckets_[idx]) {
            if (el.first == key) {
                return {true, el.second};
            }
        }
        return {false, V()};
    }

    const V At(const K& key) const {
        size_t lock_idx = GetLock(key);
        std::unique_lock<std::mutex> guard(locks_[lock_idx]);
        size_t idx = GetHash(key);
        for (auto& el : buckets_[idx]) {
            if (el.first == key) {
                return el.second;
            }
        }
        throw std::out_of_range("At operation with non-existing key");
    }

    size_t Size() const {
        return size_;
    }

    static const int kDefaultConcurrencyLevel;
    static const int kUndefinedSize;

private:
    void CheckLoadFactor() {
        if (size_ / capacity_ == 4) {
            Rehash();
        }
    }
    void Rehash() {
        for (auto& m : locks_) {
            m.lock();
        }
        capacity_ = capacity_ * 3;
        auto copy = buckets_;
        buckets_.clear();
        buckets_.resize(capacity_);
        size_ = 0;
        for (const auto& list : copy) {
            for (const auto& el : list) {
                size_t idx = GetHash(el.first);
                buckets_[idx].emplace_back(el.first, el.second);
                ++size_;
            }
        }
        for (int i = lock_capacity_ - 1; i >= 0; --i) {
            locks_[i].unlock();
        }
    }

    size_t GetLock(const K& key) const {
        return hasher_(key) % locks_.size();
    }

    size_t GetHash(const K& key) const {
        return hasher_(key) % capacity_;
    }

    std::atomic<size_t> size_ = 0;
    std::atomic<size_t> capacity_ = 8;
    std::atomic<size_t> lock_capacity_ = kDefaultConcurrencyLevel;
    mutable std::vector<std::mutex> locks_;
    std::vector<std::list<std::pair<K, V>>> buckets_;
    Hash hasher_ = Hash();
};

template <class K, class V, class Hash>
const int ConcurrentHashMap<K, V, Hash>::kDefaultConcurrencyLevel = 8;

template <class K, class V, class Hash>
const int ConcurrentHashMap<K, V, Hash>::kUndefinedSize = -1;
