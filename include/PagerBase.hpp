#pragma once
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

constexpr unsigned NAME_LEN = 12;
constexpr unsigned ITEM_LEN = 3;

struct InfoRow {
    std::string name;
    std::function<std::string(unsigned pos)> content;
    bool colored = true;
};

template <int N>
class PagerBase {
public:
    PagerBase(std::vector<unsigned> _reqs) : reqs(_reqs) {
        static_assert(N > 0);
        enroll_show_row(
            InfoRow{"pages", [this](unsigned pos) { return std::to_string(this->pages[pos]); }});
    }
    void run() {
        fout.open("out/" + std::string(this->name()) + ".txt", std::ios::out);
        if (!fout.is_open())
            return;

        fout << "\033[1;32m" << std::string(30, '=') << name() << std::string(30, '=')
             << "\033[0m\n";

        while (idx < reqs.size()) {
            for (unsigned i = 0; i < reqs.size(); i++) {
                if (i == idx)
                    fout << "\033[1;32m";
                fout << reqs[i] << " ";
                if (i == idx)
                    fout << "\033[0m";
            }
            fout << '\n';

            auto p = reqs[idx];
            hit = mp.count(p);
            hit_cnt += hit;
            auto pos = insert(p);
            idx++;

            show(pos);
        }

        fout << std::format("命中{}次, 命中率{:.2f}%\n", hit_cnt,
                            hit_cnt * 1e2 / this->reqs.size());
        fout.close();
    }
    void operator()() { run(); }

protected:
    virtual unsigned insert(unsigned p) = 0;
    virtual const char* name() const noexcept = 0;

    bool is_exists(unsigned p, unsigned& pos) {
        if (!mp.count(p))
            return false;
        pos = mp[p];
        return true;
    }

    bool try_plain_insert(unsigned p, unsigned& pos) {
        if (cnt < N) {
            mp[p] = cnt;
            pos = cnt;
            pages[cnt++] = p;
            return true;
        }
        return false;
    }

    void replace(unsigned pos, unsigned p) {
        mp.erase(pages[pos]);
        pages[pos] = p;
        mp[p] = pos;
    }

    void show(unsigned notice) {

        fout << "+" + std::string(NAME_LEN, '-') + "+";
        for (unsigned i = 0; i < N; i++)
            fout << std::string(ITEM_LEN, '-') + "+";
        fout << "\n";

        for (int i = 0; i < info_rows.size(); i++) {
            fout << std::format("|{:{}s}|", info_rows[i].name, NAME_LEN);
            for (unsigned j = 0; j < N; j++) {
                if (pages[j] == 0)
                    fout << std::string(ITEM_LEN, ' ') + "|";
                else {
                    if (j == notice && info_rows[i].colored)
                        fout << (hit ? "\033[1;32m" : "\033[1;31m");
                    fout << std::format("{:{}}", info_rows[i].content(j), ITEM_LEN) << "\033[0m|";
                }
            }
            fout << '\n';
            fout << "+" + std::string(NAME_LEN, '-') + "+";

            for (unsigned i = 0; i < N; i++)
                fout << std::string(ITEM_LEN, '-') + "+";
            fout << "\n";
        }
        fout << "\n";
    }

    void enroll_show_row(InfoRow row) { info_rows.push_back(row); }

protected:
    std::fstream fout;
    unsigned idx = 0;
    unsigned cnt = 0;
    std::vector<unsigned> reqs;
    std::array<unsigned, N> pages = {0};
    std::unordered_map<unsigned, unsigned> mp;
    std::vector<InfoRow> info_rows;

    bool hit;
    unsigned hit_cnt = 0;
};
