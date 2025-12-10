#pragma once
#include "PagerBase.hpp"
#include <array>

template <unsigned N>
class Pager_Clock : public PagerBase<N> {
public:
    Pager_Clock(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {
        this->enroll_show_row("Access Flags", this->access_flgs.data(), sizeof(unsigned));
    }

protected:
    unsigned insert(unsigned p) override {
        unsigned pos;
        if (this->mp.count(p))
            return this->mp[p];

        if (this->try_plain_insert(p, pos)) {
            this->replace(pos, p);
            this->access_flgs[pos] = true;
            return pos;
        }

        while (true) {
            this->cur = (this->cur + 1) % N;
            if (this->access_flgs[this->cur])
                this->access_flgs[this->cur] = false;
            else {
                pos = this->cur;
                break;
            }
        }
        this->replace(pos, p);
        this->access_flgs[pos] = true;
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
    std::array<unsigned, N> access_flgs;
    unsigned cur = N - 1;
};
