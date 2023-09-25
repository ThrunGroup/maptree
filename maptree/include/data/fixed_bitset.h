/**
 * @file fixed_bitset.h
 * @brief Fixed size bitset.
 * 
 * This file contains a fixed size bitset implementation. This bitset is
 * implemented as a vector of blocks (unsigned long longs) which are not
 * modified after construction.
*/

#ifndef FIXED_BITSET_H
#define FIXED_BITSET_H

#include <vector>
#include <iostream>

#include "constants.h"

/**
 * @class FixedBitset
 * @brief Fixed size bitset.
*/
class FixedBitset {
    public:
        FixedBitset(
            size_t numSamples
        )
        : numSamples_(numSamples)
        , blocks_(NUM_BLOCKS(numSamples), 0)
        {};
        
        /**
         * @brief Sets the blocks of this bitset using the provided bit vector.
         * @param bits The bit vector to set the blocks of this bitset with.
         * @pre The size of the provided bit vector must be equal to the number
         * of samples in this bitset.
         * @returns void
        */
        void setBits(
            const std::vector<bool>& bits
        );

        /**
         * @brief Returns the block at the provided index.
         * @param index The index of the block to access.
         * @returns The block at the provided index.
        */
        BLOCK getBlock(
            size_t blockIdx
        ) const;
    
    private:
        [[maybe_unused]] size_t numSamples_;
        std::vector<BLOCK> blocks_;
};

#endif
