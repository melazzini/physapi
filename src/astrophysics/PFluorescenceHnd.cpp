#include "PFluorescenceHnd.hpp"
#include <numeric>
#include "montecarlo_utils.hpp"

namespace physapi
{
    std::optional<t_fluorescentLine> PFluorescenceHnd::run(eZ z,
                                            phys_size is,
                                            phys_size st,
                                            phys_size aug)
    {
        auto lines = m_fluorescenceTable->lines(z, st, is, aug);

        if (lines.empty())
        {
            return std::nullopt;
        }

        auto yields = yieldList(lines);

        auto total_yield = std::accumulate(yields.begin(), yields.end(), 0.0);

        if (total_yield < m_randMng.random_0_1())
        {
            return std::nullopt;
        }

        std::vector<phys_float> probabilities;
        for (auto &&yield : yields)
        {
            probabilities.push_back(yield / total_yield);
        }

        phys_size indexOfFluorescenceElement = getRandomIndexMontecarlo(probabilities, m_randMng);

        return lines[indexOfFluorescenceElement];
    }

    std::vector<phys_float> PFluorescenceHnd::yieldList(const std::vector<t_fluorescentLine> &lines) const
    {
        std::vector<phys_float> yields;
        for (auto &&line : lines)
        {
            yields.push_back(line.second.lineYield());
        }

        return std::move(yields);
    }
} // namespace physapi
