/**
 * @file bitset.h
 * @brief Reversible sparse bitset.
 * 
 * This file contains an implementation of a reversible sparse bitset. This
 * bitset is implemented as a vector of reversible blocks (RNumber) and a vector
 * of indices of the blocks. The bitset is reversible in that we can apply masks
 * to it that remove bits or blocks from the bitset and then reverse the mask to
 * restore the bitset to its original state.
 * 
 * @see https://arxiv.org/abs/1604.06641
 */

#ifndef BITSET_H
#define BITSET_H

#include <cstddef>
#include <ostream>
#include <vector>
#include <algorithm>

#include "constants.h"
#include "data/rnumber.h"
#include "data/fixed_bitset.h"

/**
 * @class Bitset
 * @brief Reversible sparse bitset implementation.
 */
class Bitset {
    public:
        Bitset(
            size_t numSamples,
            size_t maxLevel
        )
        : numBlocks_(NUM_BLOCKS(numSamples))
        , maxLevel_(maxLevel)
        , blocks_(numBlocks_, RNumber(maxLevel + 1, FULL_BLOCK))
        , indices_(numBlocks_)
        , limit_(maxLevel + 1, numBlocks_)
        {
            for (size_t i = 0; i < numBlocks_; i++) indices_[i] = i;
            BLOCK lastBlock = (1ULL << (numSamples % BLOCK_BITS)) - 1;
            blocks_[numBlocks_ - 1].set(lastBlock);
        };

        /**
         * @brief Returns the current level of the bitset, or the number of 
         * masks that have been applied to it.
         * @returns The current level of the bitset.
         */
        size_t level() const;

        /**
         * @brief Returns the number of blocks in the bitset.
         * @returns The number of blocks in the bitset.
         */
        int count() const;

        /**
         * @brief Returns the number of bits in the intersection of this bitset
         * and the provided fixed bitset.
         * @param other The fixed bitset to intersect with.
         * @returns The number of bits in the intersection of this bitset and
         * the provided fixed bitset.
         */
        int countIntersection(const FixedBitset& other) const;
        
        /**
         * @brief Checks if this bitset is a subset of the provided fixed
         * bitset.
         * @param other The fixed bitset to check if this bitset is a subset of.
         * @returns True if this bitset is a subset of the provided fixed
         * bitset, false otherwise.
         */
        bool isSubset(const FixedBitset& other) const;

        /**
         * @brief Updates the bitset by applying the provided mask.
         * @param other The fixed bitset mask to apply to the bitset.
         * @returns void
         */
        void intersect(const FixedBitset& other);

        /**
         * @brief Reverses the last mask applied to the bitset.
         * @returns void
         */
        void reverse();

        /**
         * @brief Resets the bitset to its original state.
         * @returns void
         */
        void reset();

        /**
         * @brief Returns a weighted sum across all blocks of the bitset.
         * @param blockWeights The weights to use for each block.
         * @returns The weighted sum across all blocks of the bitset.
         */
        BLOCK sumOfBlocks(const std::vector<BLOCK>& blockWeights) const;

        /**
         * @brief Outputs the bitset as a string of its blocks.
         */
        friend std::ostream& operator<<(std::ostream& os, const Bitset& bitset);
    
    private:
        size_t level_ = 0;
        size_t numBlocks_;
        [[maybe_unused]] size_t maxLevel_;
        std::vector<RNumber> blocks_;
        std::vector<size_t> indices_;
        RNumber limit_;
};

#endif
