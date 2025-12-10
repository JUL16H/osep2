#pragma once
#include "PagerBase.hpp"
#include <list>

template <unsigned N>
class Pager_LRU : public PagerBase<N> {
public:
    Pager_LRU(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {
        this->enroll_show_row("Lst", [this](unsigned pos) {
            return std::to_string(this->show_arr_lst[pos]);
        });
    }

protected:
    unsigned insert(unsigned p) override {
        unsigned pos;
        if (this->is_exists(p, pos)) {
            this->lru_lst.splice(this->lru_lst.end(), this->lru_lst, this->lru_mp[p]);
            this->reset_show_info();
            return pos;
        }

        if (this->try_plain_insert(p, pos)) {
            this->lru_lst.push_back(p);
            this->lru_mp[p] = std::prev(this->lru_lst.end());
            this->reset_show_info();
            return pos;
        }

        pos = this->mp[this->lru_lst.front()];
        this->replace(pos, p);
        this->lru_mp.erase(this->lru_lst.front());
        this->lru_lst.pop_front();
        this->lru_lst.push_back(p);
        this->lru_mp[p] = std::prev(this->lru_lst.end());
        this->reset_show_info();
        return pos;
    }

    const char* name() const noexcept override { return "LRU"; }

private:
    void reset_show_info() {
        auto it = lru_lst.begin();
        for (unsigned i = 0; i < lru_lst.size(); i++)
            this->show_arr_lst[i] = *(it++);
    }

private:
    std::list<unsigned> lru_lst;
    std::unordered_map<unsigned, typename decltype(lru_lst)::iterator> lru_mp;

    std::array<unsigned, N> show_arr_lst;
};
