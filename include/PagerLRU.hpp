#pragma once
#include "PagerBase.hpp"
#include <array>

template <unsigned N>
class Pager_LRU : public PagerBase<N> {
public:
	Pager_LRU(std::vector<unsigned> _reqs) : PagerBase<N>(_reqs) {}

protected:
	unsigned insert(unsigned p) override {
		if (this->mp.count(p)) {
		}
	}

	const char* name() const noexcept override { return "LRU"; }

private:
	unsigned head;
	unsigned tail = 0;
	std::array<unsigned, N> prev = {0};
	std::array<unsigned, N> nxt = {0};
};
