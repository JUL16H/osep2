#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <format>
#include <fstream>
#include "unordered_set"

template <int N>
class PagerBase {
public:
    PagerBase(std::vector<unsigned> _reqs) : reqs(_reqs) { }
    void run() {
        fout.open("out/" + std::string(this->name()) + ".txt", std::ios::out);
        if (!fout.is_open())
            return;

        fout << "\033[1;32m" << std::string(30, '=') << name()
         << std::string(30, '=') << "\033[0m\n";

        while (idx < reqs.size()) {
            for (unsigned i = 0; i < reqs.size(); i++) {
                if (i == idx)
                    fout << "\033[1;32m";
                fout << reqs[i] << " ";
                if (i == idx)
                    fout << "\033[0m";
            }
            fout << '\n';

            auto p = reqs[idx++];
            auto pos = insert(p);

            show(pos);
        }
        fout.close();
    }
    void operator()() { run(); }

protected:
    virtual unsigned insert(unsigned p) = 0;

    unsigned plain_insert(unsigned p) {

    }

    void replace(unsigned pos, unsigned p) {

    }

    void show(unsigned notice) {
        fout << '+';
        for (unsigned i = 0; i < N; i++)
            fout << "--+";
        fout << "\n|";
        for (unsigned i = 0; i < N; i++) {
            if (pages[i] == 0)
                fout << "  |";
            else {
                if (i == notice)
                    fout << "\033[1;31m";
                fout << std::format("{:2d}", pages[i]) << "\033[0m|";
            }
        }
        fout << "\n+";
        for (unsigned i = 0; i < N; i++)
            fout << "--+";
        fout << "\n\n";
    }

protected:
    unsigned idx = 0;
    unsigned cnt = 0;
    std::fstream fout;
    std::pmr::unordered_set<unsigned> st;
    std::array<unsigned, N> pages = { 0 };
    std::vector<unsigned> reqs;
    virtual const char* name() const noexcept = 0;
};
