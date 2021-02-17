#include <mutex>
#include <condition_variable>
#include <atomic>

class RWLock {
public:
    template <class Func>
    void Read(Func func) {
        std::unique_lock<std::mutex> lock(global_);
        ++blocked_readers_;
        lock.unlock();
        try {
            func();
        } catch (...) {
            EndRead();
            throw;
        }
        EndRead();
    }

    template <class Func>
    void Write(Func func) {
        std::unique_lock<std::mutex> lock(global_);
        write_cv_.wait(lock, [this]() -> bool { return blocked_readers_ == 0; });
        func();
    }

private:
    std::mutex global_;
    std::condition_variable write_cv_;
    std::atomic<int> blocked_readers_ = 0;

    void EndRead() {
        std::unique_lock<std::mutex> lock(global_);
        --blocked_readers_;
        write_cv_.notify_all();
    }
};
