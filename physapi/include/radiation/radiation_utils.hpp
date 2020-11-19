#pragma once

#include "utils.hpp"
#include "array"
#include "tuple"
/**
 * @defgroup radiation
 * 
 * @brief   This module has tools to deal with radiation. 
 * 
 * 
 */

namespace physapi
{

    /**
     * @brief *Generic* spectrum data type.
     * 
     * A  t_spectrum  is an std::array of std::map<phys_float,phys_float>
     * where the first key is the energy value of the interval and the
     * value of the std::map is the generic spectrum value in that interval.
     * The std::array index is the index of the corresponding energy interval.
     * 
     * A generic spectrum may be a flux, a flux per energy interval, a photon distribution, etc.
     * 
     * @ingroup radiation
     * 
     * 
     */
    template <size_t arraySize>
    using t_spectrum = std::array<std::pair<phys_float, phys_float>, arraySize>;

    /**
     * @brief Spectrum scale types(linear and logarithmic)
     * 
     */
    enum class eSpectrumScale
    {
        LIN = 0, // LINEAR
        LOG = 1  // LOGARITHMIC
    };

} // namespace physapi
