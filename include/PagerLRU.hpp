#pragma once
#include "PagerBase.hpp"

template <unsigned N>
class Pager_LRU : public PagerBase<N> {
public:
    Pager_LRU(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {}

    unsigned insert(unsigned p) override {
        for (unsigned i = 0; i < N; i++)
            if (this->pages[i] == p)
                return i;

        if (this->cnt != N) {
            this->pages[this->cnt++] = p;
            return this->cnt - 1;


        }
    }

protected:
    const char* name() const noexcept override {
        return "LRU";
    }
};
