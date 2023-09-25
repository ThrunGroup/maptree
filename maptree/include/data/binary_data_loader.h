/**
 * @file binary_data_loader.h
 * @brief Loads binary data from a file.
 */

#ifndef BINARY_DATA_LOADER_H
#define BINARY_DATA_LOADER_H

#include <vector>
#include <string>

/**
 * @class BinaryDataLoader
 * @brief Loads binary data from a file.
 * 
 * Initialized with a filename, this class loads binary data from a file. The
 * file should contain only space delimited 0's and 1's, with the same number of
 * values on each line. Every line represents a single data point, and the first
 * value on each line is the label for that data point.
 */
class BinaryDataLoader {
    public:
        BinaryDataLoader(
            const std::string& filename
        )
        : filename_(filename)
        {
            load();
        }

        /**
         * @brief Returns the features loaded from the file.
         */
        const std::vector<std::vector<bool>>& getFeatures() const;

        /**
         * @brief Returns the labels loaded from the file.
         */
        const std::vector<bool>& getLabels() const;
    private:
        std::string filename_;
        std::vector<std::vector<bool>> features_;
        std::vector<bool> labels_;
        void load();
};

#endif
