#pragma once
#include <vector>
#include <iostream>
#include <format>
#include <unordered_map>
#include <algorithm>

template <int N>
class PagerBase {
public:
    PagerBase(std::vector<unsigned> _reqs) : reqs(_reqs) {};
    void run() {
        std::cout << "\033[1;32m" << std::string(30, '=') << name()
         << std::string(30, '=') << "\033[0m\n";
        while (idx < reqs.size()) {
            auto i = insert();
            show(i);
        }
    }
    void operator()() { run(); }
protected:
    unsigned insert() {
        auto p = reqs[idx++];
        if (!p)
            return 0;
        for (unsigned i = 0; i < reqs.size(); i++) {
            if (i == idx - 1)
                std::cout << "\033[1;32m";
            std::cout << reqs[i] << " ";
            if (i == idx - 1)
                std::cout << "\033[0m";
        }
        std::cout << '\n';
        for (unsigned i = 0; i < N; i++)
            if (pages[i] == p)
                return i;
        for (unsigned i = 0; i < N; i++) {
            if (!pages[i]) {
                pages[i] = p;
                return i;
            }
        }
        auto i = replace(p);
        pages[i] = p;
        return i;
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
        std::cout << "\n\n";
    }

protected:
    unsigned idx = 0;
    std::array<unsigned, N> pages = { 0 };
    std::vector<unsigned> reqs;
    virtual const char* name() const noexcept = 0;
};

template <unsigned N>
class Pager_OPT : public PagerBase<N> {
public:
    Pager_OPT(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {}

    unsigned replace(unsigned p) override {
        std::unordered_map<unsigned, unsigned> mp;
        for (unsigned i = 0; i < N; i++)
            mp[this->pages[i]] = i;

        for (unsigned i = this->idx; i < this->reqs.size(); i++) {
            if (mp.size() == 1)
                break;
            if (mp.count(this->reqs[i]))
                mp.erase(this->reqs[i]);
        }

        return mp.begin()->second;
    }

protected:
    const char* name() const noexcept override {
        return "OPT";
    }
};


template <unsigned N>
class Pager_FIFO : public PagerBase<N> {
public:
    Pager_FIFO(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {}

    unsigned replace(unsigned p) override {
        unsigned rst = cur;
        cur = (cur + 1) % N;
        return rst;
    }

protected:
    const char* name() const noexcept override {
        return "FIFO";
    }

private:
    unsigned cur = 0;
};
