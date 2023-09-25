#include <cassert>

#include "data/bitset.h"

size_t Bitset::level() const {
    return level_;
}

int Bitset::count() const {
    int count = 0;
    size_t idx;
    for (size_t i = 0; i < limit_.get(); i++) {
        idx = indices_[i];
        count += blocks_[idx].countBits();
    }
    return count;
}

int Bitset::countIntersection(const FixedBitset& other) const {
    int count = 0;
    size_t idx;
    for (size_t i = 0; i < limit_.get(); i++) {
        idx = indices_[i];
        count += blocks_[idx].countBitsAtIntersection(other.getBlock(idx));
    }
    return count;
}

bool Bitset::isSubset(const FixedBitset& other) const {
    size_t idx;
    for (size_t i = 0; i < limit_.get(); i++) {
        idx = indices_[i];
        if (!blocks_[idx].isSubset(other.getBlock(idx))) return false;
    }
    return true;
}

void Bitset::intersect(const FixedBitset& other) {
    assert(level_ + 1 < maxLevel_);
    size_t limit = limit_.get();
    size_t idx;
    if (limit == 0) return;
    for (size_t i = limit; i--;) {
        idx = indices_[i];
        blocks_[idx].intersect(other.getBlock(idx));
        if (blocks_[idx].empty()) {
            assert(limit > 0);
            limit--;
            indices_[i] = indices_[limit];
            indices_[limit] = idx;
        }
    }
    limit_.update(limit);
    level_++;
}

void Bitset::reverse() {
    size_t idx;
    limit_.reverse();
    for (size_t i = 0; i < limit_.get(); i++) {
        idx = indices_[i];
        blocks_[idx].reverse();
    }
    level_--;
}

void Bitset::reset() {
    limit_.reset();
    for (size_t i = 0; i < numBlocks_; i++) {
        blocks_[i].reset();
    }
    level_ = 0;
}

BLOCK Bitset::sumOfBlocks(const std::vector<BLOCK>& blockMults) const {
    size_t idx;
    BLOCK sum = 0;
    for (size_t i = 0; i < limit_.get(); i++) {
        idx = indices_[i];
        sum += blocks_[idx].get() * blockMults[idx];
    }
    return sum;
}

std::ostream& operator<<(std::ostream& os, const Bitset& bitset) {
    os << "[ ";
    for (size_t i = 0; i < bitset.limit_.get(); i++) {
        os << "(" << bitset.indices_[i] << ": " << bitset.blocks_[bitset.indices_[i]].get() << ") ";
    }
    os << "]";
    return os;
}