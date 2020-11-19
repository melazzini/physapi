#pragma once

#include "PSimpleElectron.hpp"
#include "PSimplePhoton.hpp"
#include "PRandomDirection.hpp"

/**
 * @defgroup compton
 * 
 * @brief This submodule handles compton interaction.
 * 
 * @ingroup interactions
 * 
 */

namespace physapi
{
    /**
     * @brief Calculate the Klein Nishina scattering cross section.
     * 
     * @param hv photon energy
     * @return phys_float cross section
     * 
     * @ingroup compton
     */
    phys_float KleinNishina(phys_float hv);

} // namespace physapi
