#pragma once
#include "PagerBase.hpp"
#include <array>
#include <unordered_map>

template <unsigned N>
class Pager_LFU : public PagerBase<N> {
public:
    Pager_LFU(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {}

protected:
    unsigned insert(unsigned p) override {
        for (unsigned i = 0; i < cnt; i++) {
            if (this->pages[i] == p) {
                visit_cnt[i]++;
                return i;
            }
        }

        if (this->cnt != N) {
            this->pages[this->cnt] = p;
            visit_cnt[this->cnt] = 0;
            this->cnt++;
            return this->cnt - 1;
        }

        unsigned pos = 0;
        for (unsigned i = 1; i < N; i++) {
            if (this->visit_cnt[i] < this->visit_cnt[pos])
                pos = i;
        }

        this->pages[pos] = p;
        this->visit_cnt[pos] = 0;
        return pos;
    }

    const char* name() const noexcept override {
        return "LFU";
    }
private:
    unsigned cnt = 0;;
    std::unordered_map<unsigned, unsigned> mp;
    std::array<unsigned, N> visit_cnt;
};
