#include "PageReqGenerator.hpp"
#include "Pagers.hpp"

int main() {
    PageReqGenerator req_generator;
    req_generator.randomize(30);
    auto vec = req_generator.get_page_req();
    // std::ranges::for_each(vec, [](const auto& item){std::cout << item << " "; });
    // std::cout << '\n';

    Pager_OPT<5>{req_generator.get_page_req()}();
    Pager_FIFO<5>{req_generator.get_page_req()}();

    return 0;
}
