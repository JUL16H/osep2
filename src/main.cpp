#include "PageReqGenerator.hpp"
#include "Pager.hpp"

int main() {
    PageReqGenerator req_generator;
    req_generator.randomize(30);
    auto vec = req_generator.get_page_req();
    std::ranges::for_each(vec, [](const auto& item){std::cout << item << " "; });
    std::cout << '\n';

    Pager_FIFO<5> page(req_generator.get_page_req());
    page.run();

    return 0;
}
