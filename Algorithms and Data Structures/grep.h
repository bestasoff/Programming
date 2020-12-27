#pragma once

#include <algorithm>
#include <optional>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <experimental/functional>
#include <vector>
#include <utf8.h>
#include <exception>

using std::optional;
namespace fs = std::filesystem;

struct GrepOptions {
    optional<size_t> look_ahead_length;
    size_t max_matches_per_line;

    GrepOptions() {
        max_matches_per_line = 10;
    }

    GrepOptions(size_t look_ahead_length) : GrepOptions() {
        this->look_ahead_length = look_ahead_length;
    }

    GrepOptions(optional<size_t> look_ahead_length, size_t max_matches_per_line) {
        this->look_ahead_length = look_ahead_length;
        this->max_matches_per_line = max_matches_per_line;
    }
};

std::string VecToStr(const std::vector<char>& vec) {
    std::string result;
    for (const auto& el : vec) {
        result += el;
    }
    return result;
}

template <class Visitor>
void Grep(const std::string& path, const std::string& pattern, Visitor visitor,
          const GrepOptions& options) {

    if (fs::exists(path) && fs::is_directory(fs::path(path))) {
        for (auto& p : fs::recursive_directory_iterator(path)) {
            if (fs::is_directory(p)) {
                continue;
            }
            if (fs::is_symlink(p)) {
                continue;
            }
            std::ifstream file(p.path());
            if (!file.good()) {
                continue;
            }
            if (file.peek() == EOF) {
                continue;
            }
            std::string read_line;
            std::vector<int> wide_pattern;
            utf8::utf8to32(pattern.begin(), pattern.end(), std::back_inserter(wide_pattern));
            std::vector<std::vector<int>> text;
            while (std::getline(file, read_line)) {
                try {
                    std::vector<int> wide_line;
                    utf8::utf8to32(read_line.begin(), read_line.end(),
                                   std::back_inserter(wide_line));
                    text.push_back(wide_line);
                } catch (const std::exception& ex) {
                    visitor.OnError(ex.what());
                    continue;
                }
            }
            for (size_t line_num = 0; line_num < text.size(); ++line_num) {
                try {
                    std::vector<int> wide_line = text[line_num];
                    utf8::utf8to32(read_line.begin(), read_line.end(),
                                   std::back_inserter(wide_line));
                    auto it = std::search(wide_line.begin(), wide_line.end(),
                                          std::boyer_moore_searcher(
                                              wide_pattern.begin(), wide_pattern.end()));
                    int matches = 0;
                    while (it != wide_line.end() && matches != options.max_matches_per_line) {
                        int column = it - wide_line.begin();
                        auto get_str = [&]() -> std::optional<std::string> {
                            if (options.look_ahead_length != std::nullopt) {
                                std::vector<char> result;
                                auto begin = it + wide_pattern.size();
                                auto end =
                                    begin +
                                    std::min(
                                        static_cast<int64_t>(std::distance(begin, wide_line.end())),
                                        static_cast<int64_t>(options.look_ahead_length.value()));
                                utf8::utf32to8(begin, end, std::back_inserter(result));
                                return VecToStr(result);
                            }
                            return std::nullopt;
                        };
                        visitor.OnMatch(p.path(), line_num + 1, column + 1, get_str());
                        ++matches;
                        it = std::search(it + 1, wide_line.end(),
                                         std::boyer_moore_searcher(
                                             wide_pattern.begin(), wide_pattern.end()));
                    }
                } catch (const std::exception& ex) {
                    visitor.OnError(ex.what());
                }
            }
        }
    } else {
        std::ifstream file(path);
        if (!file.good()) {
            return;
        }
        if (fs::is_symlink(fs::path(path))) {
            return;
        }
        std::string read_line;
        std::vector<int> wide_pattern;
        utf8::utf8to32(pattern.begin(), pattern.end(), std::back_inserter(wide_pattern));
        for (size_t line_num = 0; std::getline(file, read_line); ++line_num) {
            try {
                std::vector<int> wide_line;
                utf8::utf8to32(read_line.begin(), read_line.end(), std::back_inserter(wide_line));
                //            std::cerr << wide_line.size() << std::endl;
                auto it = std::search(wide_line.begin(), wide_line.end(),
                                      std::boyer_moore_searcher(wide_pattern.begin(),
                                                                              wide_pattern.end()));
                int matches = 0;
                while (it != wide_line.end() && matches != options.max_matches_per_line) {
                    int column = it - wide_line.begin();
                    auto get_str = [&]() -> std::optional<std::string> {
                        if (options.look_ahead_length != std::nullopt) {
                            std::vector<char> result;
                            auto begin = it + wide_pattern.size();
                            auto end =
                                begin +
                                std::min(
                                    static_cast<int64_t>(std::distance(begin, wide_line.end())),
                                    static_cast<int64_t>(options.look_ahead_length.value()));
                            utf8::utf32to8(begin, end, std::back_inserter(result));
                            return VecToStr(result);
                        }
                        return std::nullopt;
                    };
                    std::cerr << (get_str() != std::nullopt ? get_str().value() : " ") << std::endl;
                    visitor.OnMatch(path, line_num + 1, column + 1, get_str());
                    ++matches;
                    it = std::search(it + 1, wide_line.end(),
                                     std::boyer_moore_searcher(wide_pattern.begin(),
                                                                             wide_pattern.end()));
                }
            } catch (const std::exception& ex) {
                visitor.OnError(ex.what());
            }
        }
    }
}
