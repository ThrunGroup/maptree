#include "subproblem.h"

const std::vector<Split>& Subproblem::getPath() const {
    return path_;
}

const Bitset& Subproblem::getBitset() const {
    return bitset_;
}

const std::vector<size_t>& Subproblem::getValidSplits() {
    if (hasValidSplits_) return validSplits;
    validSplits.clear();
    for (size_t f = 0; f < dm_.getNumFeatures(); f++) {
        if (!bitset_.isSubset(dm_.getFeatureMask(f, false))
         && !bitset_.isSubset(dm_.getFeatureMask(f, true))) {
            validSplits.push_back(f);
        }
    }
    hasValidSplits_ = true;
    return validSplits;
}

const std::array<int, 2>& Subproblem::getLabelCounts() {
    if (hasLabelCounts_) return labelCounts_;
    int count = bitset_.count();
    labelCounts_[1] = bitset_.countIntersection(dm_.getLabelMask(true));
    labelCounts_[0] = count - labelCounts_[1];
    hasLabelCounts_ = true;
    return labelCounts_;
}

size_t Subproblem::getDepth() const {
    return path_.size();
}

void Subproblem::applySplit(size_t feature, bool value) {
    path_.push_back({feature, value});
    bitset_.intersect(dm_.getFeatureMask(feature, value));
    hasValidSplits_ = false;
    hasLabelCounts_ = false;
}

void Subproblem::revertSplit() {
    path_.pop_back();
    bitset_.reverse();
    hasValidSplits_ = false;
    hasLabelCounts_ = false;
}

void Subproblem::reset() {
    path_.clear();
    bitset_.reset();
    hasValidSplits_ = false;
    hasLabelCounts_ = false;
}

