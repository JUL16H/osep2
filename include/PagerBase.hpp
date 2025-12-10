#pragma once
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

const unsigned NAME_LEN = 12;
const unsigned ITEM_LEN = 3;

template <int N>
class PagerBase {
public:
    PagerBase(std::vector<unsigned> _reqs) : reqs(_reqs) {
        // enroll_show_row("pages", this->pages.data(), sizeof(unsigned));
        enroll_show_row("pages", [this](unsigned pos) { return std::to_string(this->pages[pos]); });
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
            auto pos = insert(p);
            idx++;

            show(pos);
        }
        fout.close();
    }
    void operator()() { run(); }

protected:
    virtual unsigned insert(unsigned p) = 0;
    virtual const char* name() const noexcept = 0;

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

        for (int i = 0; i < show_funcs.size(); i++) {
            fout << std::format("|{:{}s}|", show_row_names[i], NAME_LEN);
            for (unsigned j = 0; j < N; j++) {
                if (pages[j] == 0)
                    fout << std::string(ITEM_LEN, ' ') + "|";
                else {
                    if (j == notice)
                        fout << "\033[1;31m";
                    fout << std::format("{:{}}", show_funcs[i](j), ITEM_LEN) << "\033[0m|";
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

    void enroll_show_row(std::string name, std::function<std::string(unsigned pos)> func) {
        show_row_names.push_back(name);
        show_funcs.push_back(func);
    }

protected:
    std::fstream fout;
    unsigned idx = 0;
    unsigned cnt = 0;
    std::vector<unsigned> reqs;
    std::array<unsigned, N> pages = {0};
    std::unordered_map<unsigned, unsigned> mp;
    std::vector<std::string> show_row_names;
    std::vector<std::function<std::string(unsigned)>> show_funcs;
};
