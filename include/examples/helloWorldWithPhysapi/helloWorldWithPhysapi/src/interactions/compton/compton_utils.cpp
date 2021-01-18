#include "compton_utils.hpp"

namespace physapi
{
    phys_float KleinNishina(phys_float hv)
    {
        phys_float H = 2 * hv / mc2_e;

        //Klein Nishina formula returns the sigma compton
        //in cm^2, we need it in m^2
        return 1._cm2 * (2 * Pi * 2.8 * 2.8 * 1.0E-26 * ((1 - 4 / H - (8 / (H * H))) * std::log(1 + H) + 0.5 + 8 / H - 0.5 / (H * H + 2 * H + 1))) / H;
    }
} // namespace physapi
