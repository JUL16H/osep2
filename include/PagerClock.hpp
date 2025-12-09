#pragma once
#include "PagerBase.hpp"
#include <array>

template <unsigned N>
class Pager_Clock : public PagerBase<N> {
public:
    Pager_Clock(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {}

protected:
    unsigned insert(unsigned p) override {
        for (unsigned i = 0; i < this->cnt; i++) {
            if (this->pages[i] == p) {
                clock_insert(i, p);
                return i;
            }
        }
        if (this->cnt < N) {
            clock_insert(cnt, p);
            return this->cnt++;
        }

        unsigned pos;
        while (true) {
            this->cur = (this->cur + 1) % N;
            if (this->access_flgs[this->cur])
                this->access_flgs[this->cur] = false;
            else {
                pos = this->cur;
                break;
            }
        }
        this->clock_insert(pos, p);
        return pos;
    }

    const char* name() const noexcept override {
        return "Clock";
    }
private:
    void clock_insert(unsigned pos, unsigned p) {
        this->pages[pos] = p;
        this->access_flgs[pos] = true;
    }
private:
    std::array<bool, N> access_flgs;
    unsigned cur = N - 1;
    unsigned cnt = 0;
};
