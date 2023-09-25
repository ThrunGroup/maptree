/**
 * @file base_cache.h
 * @brief Base cache interface for subproblem caches.
 * 
 * This file contains a base interface for cache implementations that store
 * results for subproblems already explored.
 */

#ifndef BASE_CACHE_H
#define BASE_CACHE_H

#include "subproblem.h"

/**
 * @class BaseCache
 * @brief Interface for subproblem caches.
 */
class BaseCache {
    public:
        virtual ~BaseCache() = default;

        /**
         * @brief Stores the provided value for the provided subproblem.
         * @param subproblem The subproblem to store the value for.
         * @param value The value to store.
         * @returns void
         */
        virtual void put(
            Subproblem& subproblem,
            void *value
        ) = 0;

        /**
         * @brief Retrieves the value for the provided subproblem.
         * @param subproblem The subproblem to retrieve the value for.
         * @returns The value for the provided subproblem.
         */
        virtual void *get(
            Subproblem& subproblem
        ) = 0;

        /**
         * @brief Returns the number of subproblems stored in the cache.
         * @returns The number of subproblems stored in the cache.
         */
        virtual size_t size() const = 0;
};

#endif
