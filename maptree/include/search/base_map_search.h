/**
 * @file base_map_search.h
 * @brief Base class for MAP searchers.
 * 
 * This file contains the base class for MAP searchers. MAP searchers are
 * searchers which search for the maximum a posteriori (MAP) decision tree.
 * The base class handles common functionality of MAP searchers, including
 * data preprocessing.
 */

#ifndef BASE_MAP_SEARCH_H
#define BASE_MAP_SEARCH_H

#include <utility>
#include <vector>
#include <array>
#include "data/data_manager.h"
#include "solution/solution.h"
#include "posterior/tree_prior.h"
#include "posterior/tree_likelihood.h"
/**
 * @class BaseMAPSearch
 * @brief Base class for MAP searchers.
 */
class BaseMAPSearch {
    public:
        BaseMAPSearch(
            const DataManager& dm,
            const TreeLikelihood& likelihood,
            const TreePrior& prior
        )
        : dm_(dm)
        , likelihood_(likelihood)
        , prior_(prior)
        {};
        virtual ~BaseMAPSearch() = default;

        /**
         * @brief Finds the MAP decision tree.
         * @returns a Solution object containing the MAP decision tree or the
         * best tree found thus far, as well as an upper and lower bound on the
         * unnormalized log posterior probability of the tree.
         */
        virtual Solution search() = 0;

        /**
         * @brief Computes the lower bound for a subproblem based on its label
         * counts, depth, and number of valid splits.
         * @param labelCounts
         * @param depth
         * @param numValidSplits
         * @returns The lower bound.
         */
        double getLowerBound(
            const std::array<int, 2>& labelCounts,
            size_t depth,
            size_t numValidSplits = UNKNOWN_VALID_SPLITS
        ) const;

        /**
         * @brief Computes the upper bound for a subproblem based on its label
         * counts, depth, and number of valid splits.
         * @param labelCounts
         * @param depth
         * @param numValidSplits
         * @returns The lower bound.
         */
        double getUpperBound(
                const std::array<int, 2>& labelCounts,
                size_t depth,
                size_t numValidSplits = UNKNOWN_VALID_SPLITS
        ) const;
    
    protected:
        const DataManager& dm_;
        const TreeLikelihood& likelihood_;
        const TreePrior& prior_;

    private:
        static constexpr size_t UNKNOWN_VALID_SPLITS = 498126491684794917;
};

#endif
