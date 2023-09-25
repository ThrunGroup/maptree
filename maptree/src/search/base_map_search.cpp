#include "search/base_map_search.h"

constexpr size_t BaseMAPSearch::UNKNOWN_VALID_SPLITS;

double BaseMAPSearch::getLowerBound(
    const std::array<int, 2>& labelCounts,
    size_t depth,
    size_t numValidSplits
) const {
    double perfectSplitValue = -(
        prior_.logSplitProb(
                depth,
                numValidSplits == UNKNOWN_VALID_SPLITS ? 1 : numValidSplits,
                dm_.getNumFeatures())
        + 2 * prior_.logStopProb(
                depth + 1,
                0,
                dm_.getNumFeatures())
        + likelihood_.logLikelihoodPerfectSplit(labelCounts)
    );
    double stopValue = -(
        prior_.logStopProb(
                depth,
                numValidSplits == UNKNOWN_VALID_SPLITS ? 0 : numValidSplits,
                dm_.getNumFeatures())
        + likelihood_.logLikelihood(labelCounts)
    );

    return std::min(perfectSplitValue, stopValue);
}

double BaseMAPSearch::getUpperBound(
    const std::array<int, 2>& labelCounts,
    size_t depth,
    size_t numValidSplits
) const {
    return -(
        prior_.logStopProb(
                depth,
                numValidSplits == UNKNOWN_VALID_SPLITS ? 1 : numValidSplits,
                dm_.getNumFeatures())
        + likelihood_.logLikelihood(labelCounts)
    );
}