#include <cassert>

#include "data/data_manager.h"

size_t DataManager::getNumFeatures() const {
    return numFeatures_;
}

size_t DataManager::getNumSamples() const {
    return numSamples_;
}

const FixedBitset& DataManager::getFeatureMask(size_t feature, bool value) const {
    return featureMasks_[feature * 2 + value];
}

const FixedBitset& DataManager::getLabelMask(bool value) const {
    return labelMasks_[value];
}

void DataManager::buildFeatureMasks(const std::vector<std::vector<bool>>& features) {
    std::vector<bool> featureValues(numSamples_);
    for (size_t f = 0; f < numFeatures_; f++) {
        for (size_t i = 0; i < numSamples_; i++) {
            featureValues[i] = features[i][f];
        }
        featureMasks_[f * 2 + 1].setBits(featureValues);
        featureValues.flip();
        featureMasks_[f * 2].setBits(featureValues);
    }
}

void DataManager::buildLabelMasks(const std::vector<bool>& labels) {
    std::vector<bool> labelValues(labels);
    labelMasks_[1].setBits(labelValues);
    labelValues.flip();
    labelMasks_[0].setBits(labelValues);
}