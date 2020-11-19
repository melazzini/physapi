#pragma once

#include "utils.hpp"

/**
 * @defgroup montecarlo Monte Carlo
 * 
 * @brief This module offers Monte Carlo simulation tools.
 * 
 */

namespace physapi
{
    /**
     * @brief Get a Random Index of a sequence of probabilities 
     *          using a Montecarlo approach.
     * 
     * This is based on Pozdnyakov page 305
     * 
     * @tparam T - type of the sequence
     * @param sequence - sequence of probabilities, the sum of all
     *          elements of the sequence must be 1, ie the sequence
     *          must be normalize before using this function.
     * @return phys_size - random index of the sequence. It returns
     *          -1 if it was not possible to obtain any index.
     * 
     * @ingroup montecarlo
     */
    template <typename T>
    phys_size getRandomIndexMontecarlo(const T &sequence)
    {
        PRandom rndMng;
        phys_float epsilon = rndMng.random_0_1();
        if (epsilon < sequence[0])
        {
            return 0;
        }
        else
        {
            phys_float sum_low{}, sum_upp{};
            for (auto i = sequence.begin(); i != sequence.end() - 1; i++)
            {
                sum_low = std::accumulate(sequence.begin(), i + 1, 0.0);
                sum_upp = std::accumulate(sequence.begin(), i + 2, 0.0);
                if (checkRangeInclusiveLeft(sum_low, epsilon, sum_upp))
                {
                    return (i + 1) - sequence.begin();
                }
            }
        }
        return -1;
    }

    /**
     * @brief Get a Random Index of a sequence of probabilities 
     *          using a Montecarlo approach.
     * 
     * This is based on Pozdnyakov page 305
     * 
     * @tparam T - type of the sequence
     * @param sequence - sequence of probabilities, the sum of all
     *          elements of the sequence must be 1, ie the sequence
     *          must be normalize before using this function.
     * @param rndMng - random number generator reference of type PRandom.
     * @return phys_size - random index of the sequence. It returns
     *          -1 if it was not possible to obtain any index.
     * 
     * @ingroup montecarlo
     */
    template <typename T>
    phys_size getRandomIndexMontecarlo(const T &sequence, PRandom &rndMng)
    {
        phys_float epsilon = rndMng.random_0_1();
        if (epsilon < sequence[0])
        {
            return 0;
        }
        else
        {
            phys_float sum_low{}, sum_upp{};
            for (auto i = sequence.begin(); i != sequence.end() - 1; i++)
            {
                sum_low = std::accumulate(sequence.begin(), i + 1, 0.0);
                sum_upp = std::accumulate(sequence.begin(), i + 2, 0.0);
                if (checkRangeInclusiveLeft(sum_low, epsilon, sum_upp))
                {
                    return (i + 1) - sequence.begin();
                }
            }
        }
        return -1;
    }
} // namespace physapi
