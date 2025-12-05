#pragma once
#include "PagerBase.hpp"
#include <unordered_map>

template <unsigned N>
class Pager_OPT : public PagerBase<N> {
public:
    Pager_OPT(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {}

    unsigned insert(unsigned p) override {
        for (unsigned i = 0; i < N; i++)
            if (this->pages[i] == p)
                return i;

        if (this->cnt != N) {
            this->pages[this->cnt++] = p;
            return this->cnt - 1;
        }

        std::unordered_map<unsigned, unsigned> mp;
        for (unsigned i = 0; i < N; i++)
            mp[this->pages[i]] = i;

        for (unsigned i = this->idx; i < this->reqs.size(); i++) {
            if (mp.size() == 1)
                break;
            if (mp.count(this->reqs[i]))
                mp.erase(this->reqs[i]);
        }

        auto pos = mp.begin()->second;
        this->pages[pos] = p;
        return pos;
    }

protected:
    const char* name() const noexcept override {
        return "OPT";
    }
};

