#include "PageReqGenerator.hpp"
#include <algorithm>
#include <iostream>

int main() {
    PageReqGenerator req_generator;
    req_generator.randomize(30);
    auto vec = req_generator.get_page_req();
    std::ranges::for_each(vec, [](const auto& item){std::cout << item << " "; });
}
