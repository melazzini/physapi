#include "PRandomDirection.hpp"

namespace physapi
{
    void PRandomDirection::operator()(PVector3D *vect)
    {
        m_xi1 = m_randMgr.random_0_1();
        m_xi2 = m_randMgr.random_0_1();

        vect->setZ(2 * m_xi1 - 1);

        vect->setY(std::sqrt(1 - sqr(vect->z())) * std::sin(2 * Pi * m_xi2));

        vect->setX(std::sqrt(1 - sqr(vect->z())) * std::cos(2 * Pi * m_xi2));
    }

    const PVector3D &PRandomDirection::operator()()
    {
        this->operator()(&m_vector);

        return m_vector;
    }
} // namespace physapi
