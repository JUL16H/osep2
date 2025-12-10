#pragma once
#include "PagerBase.hpp"
#include <array>
#include <utility>

template <unsigned N>
class Pager_LFU : public PagerBase<N> {
public:
    Pager_LFU(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {
        this->enroll_show_row("Visit Cnt", [this](unsigned pos) {
            return std::to_string(this->visit_info[pos].first);
        });
        this->enroll_show_row("Visit Time", [this](unsigned pos) {
            return std::to_string(this->visit_info[pos].second);
        });
    }

protected:
    unsigned insert(unsigned p) override {
        unsigned pos;
        if (this->mp.count(p)) {
            pos = this->mp[p];
            visit_info[pos].first++;
            visit_info[pos].second = this->idx;
            return pos;
        }

        if (this->try_plain_insert(p, pos)) {
            visit_info[pos] = {1, this->idx};
            return pos;
        }

        pos = 0;
        for (unsigned i = 1; i < N; i++) {
            if (this->visit_info[i] < this->visit_info[pos])
                pos = i;
        }

        this->replace(pos, p);
        this->visit_info[pos] = {1, this->idx};
        return pos;
    }

    const char* name() const noexcept override { return "LFU"; }

private:
    std::array<std::pair<unsigned, unsigned>, N> visit_info;
};
