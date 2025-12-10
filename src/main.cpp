#include "PageReqGenerator.hpp"
#include "PagerClock.hpp"
#include "PagerFIFO.hpp"
#include "PagerLFU.hpp"
#include "PagerLRU.hpp"
#include "PagerOPT.hpp"

int main() {
    PageReqGenerator req_generator;
    req_generator.randomize(30);
    auto vec = req_generator.get_page_req();

    Pager_OPT<5>{vec}();
    Pager_FIFO<5>{vec}();
    Pager_LFU<5>{vec}();
    Pager_LRU<5>{vec}();
    Pager_Clock<5>{vec}();

    return 0;
}
