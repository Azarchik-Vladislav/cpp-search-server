#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

template<typename It>
class IteratorRange {
public:
    IteratorRange(It begin, It end) : page_begin_(begin),
        page_end_(end) {}
    ~IteratorRange() = default;

    auto begin_page() const {
        return page_begin_;
    }

    auto end_page() const {
        return  page_end_;
    }
    
    int size_page() const {
        return distance(page_begin_, page_end_);
    }

private:
    template<typename Iterator>
    friend std::ostream& operator<<(std::ostream& out, IteratorRange<Iterator> iter);

    It page_begin_ = nullptr;
    It page_end_ = nullptr;
};

template<typename IterForContainer>
class Paginator {
public:
    Paginator() = default;

    Paginator(IterForContainer begin, IterForContainer end, int page_size) {

        auto iter = begin;

        while (iter != end) {
            if(page_size == 0){
                pages_.clear();
                break;
            }
            auto tmp_iter = iter;
            int size_for_page = std::min(page_size, static_cast<int>(std::distance(iter, end)));

            std::advance(tmp_iter, size_for_page);

            pages_.push_back({ iter, tmp_iter });
            std::advance(iter, size_for_page);
        }
    }
    auto begin() const {
        return  pages_.begin();
    }

    auto end() const {
        return  pages_.end();
    }

    int size() const {
        return  pages_.size();
    }

private:
    std::vector<IteratorRange<IterForContainer>> pages_;
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

template<typename Iterator>
std::ostream& operator<<(std::ostream& out, IteratorRange<Iterator> iter) {
    for (auto it = iter.begin_page(); it != iter.end_page(); ++it) {
        out << *it;
    }
    return out;
}
