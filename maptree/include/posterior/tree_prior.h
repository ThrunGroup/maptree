/**
 * @file tree_prior.h
 * @brief This file contains the tree priors used in MAP tree search.
 *
 * Tree priors include:
 *     - BCART: Constructive prior which assumes a probably of splitting the
 *     tree that decreases exponentially as depth increases. This prior does not
 *     support degenerate trees, or trees consisting of leaf nodes containing no
 *     points in the training data.
 *     - BCART-degen: Same as BCART prior but this prior supports degenerate
 *     trees.
 *     - Uniform: Uniform prior across all trees. Does not support degenerate
 *     trees.
 *
 * @see https://www.jstor.org/stable/2669832
 */

#ifndef TREE_PRIOR_H
#define TREE_PRIOR_H

#include <cstddef>

/**
 * @class TreePrior
 * @brief Interface for tree priors used in MAP tree search.
 */
class TreePrior {
    public:
        virtual ~TreePrior() = default;

        virtual double logSplitProb(
            size_t depth,
            size_t numValidSplits,
            size_t numFeatures
        ) const = 0;

        virtual double logStopProb(
            size_t depth,
            size_t numValidSplits,
            size_t numFeatures
        ) const = 0;
};

class BCARTTreePrior : public TreePrior {
    public:
        BCARTTreePrior(
            double alpha,
            double beta
        )
        : alpha_(alpha)
        , beta_(beta)
        {};

        double logSplitProb(size_t depth, size_t numValidSplits, size_t numFeatures) const override;
        double logStopProb(size_t depth, size_t numValidSplits, size_t numFeatures) const override;
    private:
        double alpha_;
        double beta_;
};

class BCARTDegenTreePrior : public TreePrior {
    public:
        BCARTDegenTreePrior(
            double alpha,
            double beta
        )
            : alpha_(alpha)
            , beta_(beta)
        {};

        double logSplitProb(size_t depth, size_t numValidSplits, size_t numFeatures) const override;
        double logStopProb(size_t depth, size_t numValidSplits, size_t numFeatures) const override;
    private:
        double alpha_;
        double beta_;
};

class UniformTreePrior : public TreePrior {
    public:
        double logSplitProb(size_t depth, size_t numValidSplits, size_t numFeatures) const override;
        double logStopProb(size_t depth, size_t numValidSplits, size_t numFeatures) const override;
};

#endif
