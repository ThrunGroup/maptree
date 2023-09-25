#include <cmath>

#include "posterior/tree_prior.h"

double BCARTTreePrior::logSplitProb(
    size_t depth,
    size_t numValidSplits,
    [[maybe_unused]] size_t numFeatures
) const {
    double logSplitProb = std::log(alpha_) - beta_ * std::log(depth + 1);
    return logSplitProb - std::log(numValidSplits);
}

double BCARTTreePrior::logStopProb(
    size_t depth,
    size_t numValidSplits,
    [[maybe_unused]] size_t numFeatures
) const {
    if (numValidSplits == 0) return 0.0;
    double logSplitProb = std::log(alpha_) - beta_ * std::log(depth + 1);
    return std::log(1.0 - std::exp(logSplitProb));
}

double BCARTDegenTreePrior::logSplitProb(
    size_t depth,
    [[maybe_unused]] size_t numValidSplits,
    size_t numFeatures
) const {
    double logSplitProb = std::log(alpha_) - beta_ * std::log(depth + 1);
    return logSplitProb - std::log(numFeatures);
}

double BCARTDegenTreePrior::logStopProb(
    size_t depth,
    [[maybe_unused]] size_t numValidSplits,
    [[maybe_unused]] size_t numFeatures
) const {
    double logSplitProb = std::log(alpha_) - beta_ * std::log(depth + 1);
    return std::log(1.0 - std::exp(logSplitProb));
}

double UniformTreePrior::logSplitProb(
    [[maybe_unused]] size_t depth,
    [[maybe_unused]] size_t numValidSplits,
    [[maybe_unused]] size_t numFeatures
) const {
    return 0.0;
}

double UniformTreePrior::logStopProb(
    [[maybe_unused]] size_t depth,
    [[maybe_unused]] size_t numValidSplits,
    [[maybe_unused]] size_t numFeatures
) const {
    return 0.0;
}