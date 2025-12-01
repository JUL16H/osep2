#pragma once
#include <algorithm>
#include <vector>
#include <iostream>
#include <format>

template <int N>
class PagerBase {
public:
    PagerBase(std::vector<unsigned> _reqs) : reqs(_reqs) {};
    void run() {
        while (idx < reqs.size()) {
            auto i = insert();
            show(i);
        }
    }
protected:
    unsigned insert() {
        auto p = reqs[idx++];
        if (!p)
            return 0;
        std::cout << "req:" << p << "\n";
        for (unsigned i = 0; i < N; i++)
            if (pages[i] == p)
                return i;
        for (unsigned i = 0; i < N; i++) {
            if (!pages[i]) {
                pages[i] = p;
                return i;
            }
        }
        return replace(p);
    }

    virtual unsigned replace(unsigned p) = 0;

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
        std::cout << "\n";
    }

protected:
    unsigned idx = 0;
    std::array<unsigned, N> pages = { 0 };
    std::vector<unsigned> reqs;
};

template <unsigned N>
class Pager_FIFO : public PagerBase<N> {
public:
    Pager_FIFO(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {}

    unsigned replace(unsigned p) override {
        this->pages[cur] = p;
        unsigned rst = cur;
        cur = (cur + 1) % N;
        return rst;
    }

private:
    unsigned cur = 0;
};
