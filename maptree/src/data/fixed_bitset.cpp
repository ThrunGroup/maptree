#include <cassert>

#include "data/fixed_bitset.h"

void FixedBitset::setBits(const std::vector<bool>& bits) {
    assert(bits.size() == numSamples_);
    for (size_t i = 0; i < blocks_.size(); i++) {
        for (size_t j = 0; j < BLOCK_BITS; j++) {
            size_t idx = i * BLOCK_BITS + j;
            if (idx >= bits.size()) break;
            if (bits[idx]) {
                blocks_[i] |= 1ULL << j;
            }
        }
    }
}

BLOCK FixedBitset::getBlock(size_t idx) const {
    return blocks_[idx];
}