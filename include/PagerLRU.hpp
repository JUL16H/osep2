#pragma once
#include "PagerBase.hpp"
#include <unordered_map>
#include <deque>
#include <vector>
#include <utility>

template <unsigned N>
class Pager_LRU : public PagerBase<N> {
public:
    Pager_LRU(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {}

    unsigned insert(unsigned p) override {
        for (unsigned i = 0; i < cnt; i++) {
            if (this->pages[i] == p) {
            }
        }
    }

protected:
    const char* name() const noexcept override {
        return "LRU";
    }

private:
    unsigned cnt = 0;
    std::unordered_map<unsigned, unsigned> mp;
    std::deque<unsigned> que;
};
