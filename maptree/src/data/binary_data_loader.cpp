#include <fstream>
#include "data/binary_data_loader.h"

const std::vector<std::vector<bool>>& BinaryDataLoader::getFeatures() const {
    return features_;
}

const std::vector<bool>& BinaryDataLoader::getLabels() const {
    return labels_;
}

void BinaryDataLoader::load() {
    std::ifstream file(filename_);
    if (!file) {
        throw std::runtime_error("Could not open file " + filename_);
    }
    
    std::string line;
    size_t lastLineSize = 0;
    size_t lineNum = 0;
    while (std::getline(file, line)) {
        std::vector<bool> sample;
        for (char c : line) {
            if (c == '0' || c == '1') sample.push_back(c == '1');
        }
        if (sample.size() < 2) continue;
        if (lastLineSize != 0 && sample.size() != lastLineSize) {
            throw std::runtime_error("Inconsistent sample size on line " + std::to_string(lineNum) + " of " + filename_);
        }
        features_.push_back({sample.begin() + 1, sample.end()});
        labels_.push_back(sample[0]);
        lastLineSize = sample.size();
        lineNum++;
    }
    file.close();
}