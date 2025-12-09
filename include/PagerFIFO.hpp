#pragma once
#include "PagerBase.hpp"

template <unsigned N>
class Pager_FIFO : public PagerBase<N> {
public:
    Pager_FIFO(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {}

protected:
    unsigned insert (unsigned p) override {
        for (unsigned i = 0; i < cnt; i++)
            if (this->pages[i] == p)
                return i;

        if (this->cnt != N) {
            this->pages[this->cnt++] = p;
            return this->cnt - 1;
        }

        unsigned pos = cur;
        cur = (cur + 1) % N;

        this->pages[pos] = p;
        return pos;
    }

    const char* name() const noexcept override {
        return "FIFO";
    }

private:
    unsigned cur = 0;
    unsigned cnt = 0;
};
