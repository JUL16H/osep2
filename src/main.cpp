#include "PageReqGenerator.hpp"
#include "PagerClock.hpp"
#include "PagerFIFO.hpp"
#include "PagerLFU.hpp"
#include "PagerLRU.hpp"
#include "PagerOPT.hpp"

constexpr unsigned num_page = 5;

int main() {
    PageReqGenerator req_generator;
    req_generator.randomize(30);
    auto vec = req_generator.get_page_req();

    Pager_OPT<num_page>{vec}();
    Pager_FIFO<num_page>{vec}();
    Pager_LFU<num_page>{vec}();
    Pager_LRU<num_page>{vec}();
    Pager_Clock<num_page>{vec}();

    return 0;
}
