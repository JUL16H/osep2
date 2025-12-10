#pragma once
#include "PagerBase.hpp"

template <unsigned N>
class Pager_FIFO : public PagerBase<N> {
public:
    Pager_FIFO(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {
        this->enroll_show_row("Cur Point", this->show_cur_arr.data(),
                              sizeof(unsigned));
        this->show_cur_arr[N - 1] = 1;
    }

protected:
    unsigned insert(unsigned p) override {
        if (this->mp.count(p))
            return this->mp[p];

        unsigned pos;
        if (this->try_plain_insert(p, pos))
            return pos;

        show_cur_arr[cur] = 0;
        cur = (cur + 1) % N;
        show_cur_arr[cur] = 1;
        this->replace(cur, p);
        return cur;
    }

    const char* name() const noexcept override { return "FIFO"; }

private:
    unsigned cur = N - 1;
    std::array<unsigned, N> show_cur_arr = {0};
};
