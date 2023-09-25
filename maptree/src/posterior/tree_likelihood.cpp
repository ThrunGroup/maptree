#include <algorithm>
#include <cmath>
#include <array>

#include "posterior/tree_likelihood.h"

 double TreeLikelihood::logLikelihood(const std::array<int, 2>& labelCounts) const {
     return logBeta(
         static_cast<double>(labelCounts[0]) + rho_[0],
         static_cast<double>(labelCounts[1]) + rho_[1]
     ) - logBeta(rho_[0], rho_[1]);
}

double TreeLikelihood::logLikelihoodPerfectSplit(const std::array<int, 2>& labelCounts) const {
    return logBeta(static_cast<double>(labelCounts[0]) + rho_[0], rho_[1]) \
        + logBeta(rho_[0], static_cast<double>(labelCounts[1]) + rho_[1]) \
        - 2 * logBeta(rho_[0], rho_[1]);
}