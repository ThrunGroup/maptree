/** 
 * @file tree_likelihood.h
 * @brief This file contains the tree likelihoods used in MAP tree search.
 * 
 * This file contains the functions used to compute the likelihood of a given
 * tree based on the BCART statistical model for use in MAP tree search.
 * 
 * @see https://www.jstor.org/stable/2669832
 */

#ifndef TREE_LIKELIHOOD_H
#define TREE_LIKELIHOOD_H

#include <cstddef>
#include <vector>
#include <array>
#include <cmath>

/**
 * @class TreeLikelihood
 * @brief Contains the likelihood function for the leaves of a BCART tree as
 * well as a couple other utility functions.
 */
class TreeLikelihood {
    public:
        TreeLikelihood(
            const std::array<double, 2> rho
        )
        : rho_(rho)
        {};

        /**
         * @brief Computes the natural log of the Beta function.
         * @param alpha The first parameter of the Beta function.
         * @param beta The second parameter of the Beta function.
         * @returns The natural log of the Beta function.
         */
        static double logBeta(
            double alpha,
            double beta
        ) {
            return std::lgamma(alpha) + std::lgamma(beta) - std::lgamma(alpha + beta);
        }

        /**
         * @brief Computes the natural log likelihood of the provided binary label
         * counts in a particular leaf node.
         * @param labelCounts The number of points in the leaf node with each binary
         * label.
         * @returns The natural log likelihood of the provided binary label counts
         */
        double logLikelihood(
            const std::array<int, 2>& labelCounts
        ) const;

        /**
         * @brief Computes the natural log likelihood of a perfect split of the
         * provided binary label counts.
         * @param labelCounts The count of points with each binary label.
         * @param rho The index of the Beta distribution prior for the Bernoulli
         * distribution in the node.
         * @returns The natural log likelihood of a perfect split of the.
         */
        double logLikelihoodPerfectSplit(
            const std::array<int, 2>& labelCounts
        ) const;


    private:
        std::array<double, 2> rho_;
};

#endif
