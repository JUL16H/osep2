#pragma once
#include <vector>
#include <iostream>
#include <format>
#include "unordered_set"

template <int N>
class PagerBase {
public:
    PagerBase(std::vector<unsigned> _reqs) : reqs(_reqs) {};
    void run() {
        std::cout << "\033[1;32m" << std::string(30, '=') << name()
         << std::string(30, '=') << "\033[0m\n";

        while (idx < reqs.size()) {
            for (unsigned i = 0; i < reqs.size(); i++) {
                if (i == idx)
                    std::cout << "\033[1;32m";
                std::cout << reqs[i] << " ";
                if (i == idx)
                    std::cout << "\033[0m";
            }
            std::cout << '\n';

            auto p = reqs[idx++];
            auto pos = insert(p);

            show(pos);
        }
    }
    void operator()() { run(); }

protected:
    virtual unsigned insert(unsigned p) = 0;

    unsigned plain_insert(unsigned p) {

    }

    void replace(unsigned pos, unsigned p) {

    }

    void show(unsigned notice) {
        std::cout << '+';
        for (unsigned i = 0; i < N; i++)
            std::cout << "--+";
        std::cout << "\n|";
        for (unsigned i = 0; i < N; i++) {
            if (pages[i] == 0)
                std::cout << "  |";
            else {
                if (i == notice)
                    std::cout << "\033[1;31m";
                std::cout << std::format("{:2d}", pages[i]) << "\033[0m|";
            }
        }
        std::cout << "\n+";
        for (unsigned i = 0; i < N; i++)
            std::cout << "--+";
        std::cout << "\n\n";
    }

protected:
    unsigned idx = 0;
    unsigned cnt = 0;
    std::pmr::unordered_set<unsigned> st;
    std::array<unsigned, N> pages = { 0 };
    std::vector<unsigned> reqs;
    virtual const char* name() const noexcept = 0;
};
