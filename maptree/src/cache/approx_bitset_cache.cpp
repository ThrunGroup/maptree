#include "cache/approx_bitset_cache.h"
#include <iostream>

constexpr std::array<BLOCK, APPROX_BITSET_CACHE_NUM_ULL_HASH_VALUES> ApproxBitsetCache::BLOCK_MULT_BASE;
constexpr BLOCK ApproxBitsetCache::DEPTH_MULT;

bool operator==(const ApproxBitsetCacheKey &lhs, const ApproxBitsetCacheKey &rhs) {
    return lhs.hashedBitset == rhs.hashedBitset && lhs.depth == rhs.depth;
}

size_t ApproxBitsetCacheKeyHash::operator()(const ApproxBitsetCacheKey &key) const {
    size_t hash = key.depth * ApproxBitsetCache::DEPTH_MULT;
    for (size_t i = 0; i < APPROX_BITSET_CACHE_NUM_ULL_HASH_VALUES; i++) {
        hash ^= key.hashedBitset[i];
    }
    return hash;
}

void ApproxBitsetCache::put(Subproblem& subproblem, void *value) {
    ApproxBitsetCacheKey key = constructKey(subproblem);
    cache_.insert({key, value});
}

void *ApproxBitsetCache::get(Subproblem& subproblem) {
    ApproxBitsetCacheKey key = constructKey(subproblem);
    auto entry = cache_.find(key);
    if (entry == cache_.end()) return nullptr;
    return entry->second;
}

size_t ApproxBitsetCache::size() const {
    return cache_.size();
}

ApproxBitsetCacheKey ApproxBitsetCache::constructKey(Subproblem& subproblem) const {
    ApproxBitsetCacheKey key;
    key.depth = subproblem.getDepth();
    for (size_t i = 0; i < APPROX_BITSET_CACHE_NUM_ULL_HASH_VALUES; i++) {
        key.hashedBitset[i] = subproblem.getBitset().sumOfBlocks(blockMults_[i]);
    }
    return key;
}

void ApproxBitsetCache::initBlockMults(size_t numBlocks) {
    for (size_t p = 0; p < APPROX_BITSET_CACHE_NUM_ULL_HASH_VALUES; p++) {
        blockMults_[p].resize(numBlocks);
        blockMults_[p][0] = BLOCK_MULT_BASE[p];
        for (size_t i = 1; i < numBlocks; i++) {
            blockMults_[p][i] = blockMults_[p][i - 1] * blockMults_[p][0];
        }
    }
}
