#pragma once
#include "RNG.hpp"
#include <vector>
#include <span>
#include <algorithm>

class PageReqGenerator {
public:
    PageReqGenerator(double _repeat_p = .2, unsigned _page_num = 10) :
     repeat_p(_repeat_p), page_num(_page_num) {}
    void randomize(unsigned n) {
        std::vector<unsigned> st;
        page_req_vec.resize(n);
        for (unsigned i = 0; i < n; i++) {
            if (i>2 && rng.happen(repeat_p)) {
                std::ranges::sort(st);
                st.erase(std::unique(st.begin(), st.end()), st.end());
                page_req_vec[i] = st[rng.randu(0, st.size() - 1)];
            }
            else {
                page_req_vec[i] = rng.randu(1, page_num);
                st.push_back(page_req_vec[i]);
            }
        }
    }

    const std::vector<unsigned>& get_page_req() const noexcept {
        return page_req_vec;
    }

private:
    RNG rng;
    double repeat_p;
    unsigned page_num;
    std::vector<unsigned> page_req_vec;
};
