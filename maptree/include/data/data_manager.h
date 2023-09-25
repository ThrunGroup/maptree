/**
 * @file data_manager.h
 * @brief Precomputes information about data for searchers.
 * 
 * This file contains the DataManager class, which precomputes feature and label
 * masks for the searchers.
*/

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <vector>
#include <array>
#include <limits>

#include "data/fixed_bitset.h"

/**
 * @class DataManager
 * @brief Precomputes information about data for searchers.
 * 
 * This class uses the provided features and labels to precompute feature and
 * label masks for the searchers. These masks are stored as vectors of blocks
 * (unsigned long longs) and used by searchers to quickly update subproblems. 1
 * bits in label masks indicate that the point at that bit's index contains the
 * mask's label or feature value and a 0 bit indicates that it does not.
 */
class DataManager {
    public:
        DataManager(
            const std::vector<std::vector<bool>>& features,
            const std::vector<bool>& labels
        )
        : numFeatures_(features[0].size())
        , numSamples_(features.size())
        , featureMasks_(numFeatures_ * 2, FixedBitset(numSamples_))
        , labelMasks_(2, FixedBitset(numSamples_))
        {
            buildFeatureMasks(features);
            buildLabelMasks(labels);
        }

        /**
         * @brief Returns the number of features in the data.
        */
        size_t getNumFeatures() const;

        /**
         * @brief Returns the number of samples in the data.
         */
        size_t getNumSamples() const;

        /**
         * @brief Returns the mask for the provided feature and feature value.
         * @param feature The feature to get the mask for.
         * @param value The feature value to get the mask for.
         * @returns The mask for the provided feature and feature value.
         */
        const FixedBitset& getFeatureMask(
            size_t feature,
            bool value
        ) const;

        /**
         * @brief Returns the mask for the provided label value.
         * @param value The label value to get the mask for.
         * @returns The mask for the provided label value.
         */
        const FixedBitset& getLabelMask(
            bool value
        ) const;

    private:
        size_t numFeatures_;
        size_t numSamples_;
        std::vector<FixedBitset> featureMasks_;
        std::vector<FixedBitset> labelMasks_;

        void buildFeatureMasks(const std::vector<std::vector<bool>>& features);
        void buildLabelMasks(const std::vector<bool>& labels);
};

#endif
