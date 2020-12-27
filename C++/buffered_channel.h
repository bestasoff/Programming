#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>

template<class T>
class Channel {
public:
    explicit Channel(size_t buffer_size) : buffer_size_(buffer_size), channel_is_open(true) {}
   ~Channel() = default;

    void Send(T value) {
        std::lock_guard<std::mutex> locker(lock_queue_);
        fullqueue_.wait(locker, [this] () {
            if(!channel_is_open) {
                throw std::runtime_error("Channel is closed, but you are trying to send smth.");
            }
            return container_.size() < buffer_size_;
        });
        container_.push(value);
        emptyqueue_.notify_all();
    }

    std::pair<T, bool> Recv() {
        std::lock_guard<std::mutex> locker(lock_queue_);
        emptyqueue_.wait(locker, [&]() {
            return !container_.empty() || !channel_is_open;
        });
        if(!container_.empty() ) {
            if(channel_is_open) {
                fullqueue_.notify_one();
            }
            T tmp = container_.front();
            container_.pop();
            return {tmp, true};
        } else {
            return {T(), false};
        }
    }

    void Close() {
        std::lock_guard<std::mutex> locker(lock_queue_);
        channel_is_open = false;
        fullqueue_.notify_all();
        emptyqueue_.notify_all();
    }

private:
    size_t buffer_size_;
    std::queue<T> container_;
    std::mutex lock_queue_;
    std::condition_variable fullqueue_, emptyqueue_;
    bool channel_is_open;
};
