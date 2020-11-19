#include "astrophysics_utils.hpp"

namespace physapi
{
    std::ostream &operator<<(std::ostream &os, eZ z)
    {
        return (os << static_cast<phys_size>(z));
    }
} // namespace physapi
