/**
 * @file approx_bitset_cache.h
 * @brief Subproblem cache based on bitset hashes.
 * 
 * This file contains a cache implementation which stores results for
 * subproblems based on several hash values of their bitset and depth. Note that
 * this cache may return false positives, but due to the limited number of
 * queries and the large hash values, this is unlikely.
*/

#ifndef APPROX_BITSET_CACHE_H
#define APPROX_BITSET_CACHE_H

#include <cstddef>
#include <unordered_map>
#include <array>

#include "constants.h"
#include "cache/base_cache.h"
#include "data/bitset.h"

//! Number of unsigned long longs in the ApproxBitsetCacheKey.
#define APPROX_BITSET_CACHE_NUM_ULL_HASH_VALUES 2

/**
 * @struct ApproxBitsetCacheKey
 * @brief Key for the ApproxBitsetCache.
 * 
 * This struct contains the key for ApproxBitsetCache: the bitset hash values
 * and the depth.
 */
struct ApproxBitsetCacheKey {
    std::array<unsigned long long, APPROX_BITSET_CACHE_NUM_ULL_HASH_VALUES> hashedBitset;
    size_t depth = 0;
    friend bool operator==(const ApproxBitsetCacheKey &lhs, const ApproxBitsetCacheKey &rhs);
};

/**
 * @struct ApproxBitsetCacheKeyHash
 * @brief Hash function for ApproxBitsetCacheKey.
 */
struct ApproxBitsetCacheKeyHash {
    size_t operator()(const ApproxBitsetCacheKey &key) const;
};

/**
 * @class ApproxBitsetCache
 * @brief Subproblem cache based on hashed bitsets.
 * @implements BaseCache
 */
class ApproxBitsetCache : BaseCache {
    public:
        ApproxBitsetCache(
            size_t numBlocks
        ) {
            initBlockMults(numBlocks);
        };

        /**
         * @implements BaseCache::put
         */
        void put(
            Subproblem& subproblem,
            void *value
        ) override;

        /**
         * @implements BaseCache::get
         */
        void *get(
            Subproblem& subproblem
        ) override;

        /**
         * @implements BaseCache::size
         */
        size_t size() const override;

        static constexpr std::array<BLOCK, APPROX_BITSET_CACHE_NUM_ULL_HASH_VALUES> BLOCK_MULT_BASE = {
            377424577268497867ULL,
            285989758769553131ULL,
        };
        static constexpr BLOCK DEPTH_MULT = 234902547182092241ULL;
        
    private:
        std::unordered_map<ApproxBitsetCacheKey, void *, ApproxBitsetCacheKeyHash> cache_;
        std::array<std::vector<BLOCK>, APPROX_BITSET_CACHE_NUM_ULL_HASH_VALUES> blockMults_;
        
        void initBlockMults(size_t numBlocks);
        ApproxBitsetCacheKey constructKey(Subproblem& subproblem) const;
};

#endif
