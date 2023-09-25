/**
 * @file split.h
 * @brief Contains a struct which represents a feature-value split.
 */

#include <cstddef>

#ifndef SPLIT_H
#define SPLIT_H

/**
 * @struct Split
 * @brief Represents a feature-value split.
 */
struct Split {
    size_t feature;
    bool value;
};

#endif
