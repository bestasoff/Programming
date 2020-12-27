#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <iostream>

class LruCache {
public:
    LruCache(size_t max_size) : max_size_(max_size), data_(max_size) {
    }

    void Set(const std::string& key, const std::string& value) {
        if (map_.find(key) == map_.end()) {
            if (list_.size() == max_size_) {
                std::string last = list_.back();
                list_.pop_back();
                map_.erase(last);
                data_.erase(last);
            }
        } else {
            list_.erase(map_[key]);
            data_.erase(key);
        }
        list_.push_front(key);
        map_[key] = list_.begin();
        data_[key] = value;
    }

    bool Get(const std::string& key, std::string* value) {
        if (!data_.count(key)) {
            return false;
        }

        *value = data_[key];
        list_.erase(map_[key]);
        list_.push_front(key);
        map_[key] = list_.begin();
        return true;
    }

private:
    int max_size_;
    std::list<std::string> list_;
    std::unordered_map<std::string, std::string> data_;
    std::unordered_map<std::string, std::list<std::string>::iterator> map_;
};
