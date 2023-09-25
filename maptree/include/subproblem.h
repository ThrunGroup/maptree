/**
 * @file subproblem.h
 * @brief Decision tree search subproblem.
 * 
 * This file contains a class for storing a subproblem for the decision tree
 * search problem. A subproblem is defined by a list of splits that have been
 * applied to the root node and the set of points remaining which is represented
 * by a Bitset object.
 */

#ifndef SUBPROBLEM_H
#define SUBPROBLEM_H

#include <vector>
#include <array>
#include "data/split.h"
#include "data/bitset.h"
#include "data/data_manager.h"

/**
 * @class Subproblem
 * @brief A subproblem for the decision tree search problem.
 */
class Subproblem {
    public:
        Subproblem(
            const DataManager& dm
        )
        : dm_(dm)
        , path_()
        , bitset_(dm.getNumSamples(), dm.getNumFeatures())
        {};

        /**
         * @brief Returns the list of splits that this subproblem has taken.
         * @returns The list of this subproblem's splits
         */
        const std::vector<Split>& getPath() const;

        /**
         * @brief Returns the subproblem's bitset.
         * @returns The subproblem's bitset.
         */
        const Bitset& getBitset() const;

        /**
         * @brief Returns the features on which this subproblem can be split.
         * @returns A list of the features on which this subproblem can be
         * split.
         */
        const std::vector<size_t>& getValidSplits();

        /**
         * @brief Returns the label counts of the subproblem.
         * @returns The label counts for the subproblem.
         */
        const std::array<int, 2>& getLabelCounts();

        /**
         * @brief Returns the depth of the subproblem.
         * @returns The depth of the subproblem.
         */
        size_t getDepth() const;

        /**
         * @brief Applies the provided feature split to the given subproblem
         * choosing the subsubproblem with the provided value.
         * @param value
         */
        void applySplit(
            size_t feature,
            bool value
        );

        /**
         * @brief Reverts the last split applied to the subproblem.
         * @returns void
         */
        void revertSplit();

        /**
         * @brief Resets subproblem to initial state.
         * 
         * This method resets the subproblem to its initial state by resetting
         * its bitset and clearing its split list.
         */
        void reset();

    private:
        const DataManager& dm_;
        std::vector<Split> path_;
        Bitset bitset_;
        std::array<int, 2> labelCounts_;
        std::vector<size_t> validSplits;

        bool hasLabelCounts_ = false;
        bool hasValidSplits_ = false;
};

#endif
