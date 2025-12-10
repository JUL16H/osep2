#pragma once
#include "PagerBase.hpp"
#include <unordered_set>

template <unsigned N>
class Pager_OPT : public PagerBase<N> {
public:
    Pager_OPT(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {}

protected:
    unsigned insert(unsigned p) override {
        if (this->mp.count(p))
            return this->mp[p];

        unsigned pos;
        if (this->try_plain_insert(p, pos))
            return pos;

        std::unordered_set<unsigned> st;
        for (const auto p : this->pages)
            st.insert(p);

        for (unsigned i = this->idx; i < this->reqs.size(); i++) {
            if (st.count(this->reqs[i]))
                st.erase(this->reqs[i]);
            if (st.size() == 1)
                break;
        }

        pos = this->mp[*st.begin()];
        this->replace(pos, p);
        return pos;
    }

    const char* name() const noexcept override { return "OPT"; }
};
