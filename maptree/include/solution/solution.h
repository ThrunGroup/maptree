/**
 * @file solution.h
 * @brief Defines the Solution struct.
 * 
 * This file contains the definition of the Solution struct, which is used to
 * return the results of a search. The Solution struct contains the unnormalized
 * log posterior upper and lower bounds and a string representation of the
 * output tree.
*/

#ifndef SOLUTION_H
#define SOLUTION_H

#include <string>

/**
 * @struct Solution
 * @brief Contains the results of a search.
 */
struct Solution {
    double lowerBound;
    double upperBound;
    std::string treeRepresentation;
};

#endif