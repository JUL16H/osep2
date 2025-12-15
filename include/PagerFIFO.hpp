#pragma once
#include "PagerBase.hpp"

template <unsigned N>
class Pager_FIFO : public PagerBase<N> {
public:
    Pager_FIFO(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {
        this->enroll_show_row(InfoRow{"Cur Point", [this](unsigned pos) {
            return (this->cnt < N ? this->cnt - 1 : this->cur) == pos ? "↑" : "";
        }});
    }

protected:
    unsigned insert(unsigned p) override {
        unsigned pos;
        if (this->is_exists(p, pos) || this->try_plain_insert(p, pos))
            return this->mp[p];

        cur = (cur + 1) % N;
        this->replace(cur, p);
        return cur;
    }

    const char* name() const noexcept override { return "FIFO"; }

private:
    unsigned cur = N - 1;
};
