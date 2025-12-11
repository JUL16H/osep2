#pragma once
#include "PagerBase.hpp"
#include <list>
#include <utility>

template <unsigned N>
class Pager_LFU : public PagerBase<N> {
public:
    Pager_LFU(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {
        this->enroll_show_row(
            InfoRow{"Lst", [this](unsigned pos) { return std::to_string(this->show_arr_lst[pos]); },
                    false});
        this->enroll_show_row(InfoRow{
            "Freq", [this](unsigned pos) { return std::to_string(this->show_arr_freq[pos]); },
            false});
    }

protected:
    unsigned insert(unsigned p) override {
        unsigned pos;
        if (this->is_exists(p, pos)) {
            auto it = this->key_mp[p];
            auto freq = it->second;
            if (freq == this->min_freq && this->freq_mp[freq].size() == 1)
                this->min_freq++;
            freq++;
            this->key_mp.erase(p);
            this->freq_mp[freq].push_back({it->first, it->second + 1});
            this->freq_mp[freq - 1].erase(it);
            this->key_mp[p] = std::prev(this->freq_mp[freq].end());
            cal_show_arr();
            return pos;
        }
        if (this->try_plain_insert(p, pos)) {
            this->min_freq = 1;
            this->freq_mp[1].push_back({p, 1});
            this->key_mp[p] = std::prev(this->freq_mp[1].end());
            cal_show_arr();
            return pos;
        }
        auto old_page = this->freq_mp[this->min_freq].begin()->first;
        this->key_mp.erase(old_page);
        this->freq_mp[this->min_freq].pop_front();
        this->replace(this->mp[old_page], p);
        this->min_freq = 1;
        this->freq_mp[1].push_back({p, 1});
        this->key_mp[p] = std::prev(this->freq_mp[1].end());
        cal_show_arr();
        return this->mp[p];
    }

    const char* name() const noexcept override { return "LFU"; }

private:
    void cal_show_arr() {
        auto cur_freq = this->min_freq;
        auto it = this->freq_mp[cur_freq].begin();
        for (unsigned i = 0; i < this->cnt; i++) {
            if (it == this->freq_mp[cur_freq].end()) {
                while (this->freq_mp[++cur_freq].empty());
                it = this->freq_mp[cur_freq].begin();
            }
            this->show_arr_lst[i] = it->first;
            this->show_arr_freq[i] = it->second;
            it++;
        }
    }

private:
    unsigned min_freq = 0;
    std::unordered_map<unsigned, std::list<std::pair<unsigned, unsigned>>> freq_mp;
    std::unordered_map<unsigned, std::list<std::pair<unsigned, unsigned>>::iterator> key_mp;

    std::array<unsigned, N> show_arr_lst;
    std::array<unsigned, N> show_arr_freq;
};
