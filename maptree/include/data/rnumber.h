/**
 * @file rnumber.h
 * @brief This file contains the reversible number class used in reversible 
 * sparse bitsets.
 * 
 * This file contains the reversible number class used in reversible sparse
 * bitsets. Several actions can be performed on a reversible number, including
 * setting, updating, intersecting, reversing, resetting, and counting bits. The
 * reversible number is implemented as a vector of blocks (unsigned long longs)
 * with a head index. The head index is used to keep track of the current
 * position in the vector. Reversing can be done by simply decrementing the head
 * index in constant time.
 */

#ifndef RNUMBER_H
#define RNUMBER_H

#include <cstddef>
#include <vector>

#include "constants.h"

class RNumber {
    public:
        RNumber(
            size_t capacity,
            BLOCK initValue = 0
        )
        : values_(capacity, initValue)
        {};

        /**
         * @brief Returns the current value of the reversible number.
         */
        BLOCK get() const;

        /**
         * @brief Sets the current value to the provided value
         * @param value The value to set the reversible number to.
         * 
         * This action is irreversible.
         */
        void set(
            BLOCK value
        );

        /**
         * @brief Updates the current value to the provided value.
         * @param value The value to update the reversible number with.
         * 
         * This action is reversible.
         */
        void update(
            BLOCK value
        );

        /**
         * @brief Intersects the current value with the provided value.
         * @param value The value to intersect the reversible number with.
         * 
         * This action is reversible.
         */
        void intersect(
            BLOCK other
        );

        /**
         * @brief Reverts previous action.
         */
        void reverse();

        /**
         * @brief Resets the reversible number to its initial state.
         */
        void reset();

        /**
         * @brief Counts the number of bits in the current value.
         * @returns The number of bits in the current value.
         */
        int countBits() const;

        /**
         * @brief Counts the number of bits at the intersection of the current
         * value and the provided value.
         * @param other The value to intersect the reversible number with.
         * @returns The number of bits at the intersection of the current value
         * and the provided value.
         */
        int countBitsAtIntersection(
            BLOCK other
        ) const;

        /**
         * @brief Checks if the current value is a subset of the provided value.
         * @param other The value to check if the current value is a subset of.
         * @returns True if the current value is a subset of the provided value,
         * false otherwise.
         */
        bool isSubset(
            BLOCK other
        ) const;

        /**
         * @brief Checks if the current value is 0
         * @returns True if the current value is 0, false otherwise.
         */
        bool empty() const;
    private:
        size_t head_ = 0;
        std::vector<BLOCK> values_;
};

#endif