#include "PVector2D.hpp"
#include <iostream>

namespace physapi
{
    /***********************************************************************
    *              CONSTRUCTORS & DESTRUCTOR
    ***********************************************************************/

    PVector2D::PVector2D(phys_float x1,
                         phys_float x2,
                         eCoordSys coordSys)
        : m_x{}, m_y{}, m_currentCoordSys{}
    {
        setCoord(x1, x2, coordSys);
    }

    PVector2D::PVector2D()
        : m_x{}, m_y{}, m_currentCoordSys{}
    {
        setCoord(0, 0, eCoordSys::REC);
    }

    void PVector2D::setCoord(phys_float x1,
                             phys_float x2,
                             eCoordSys coordSys_)
    {
        setCoordSys(coordSys_);

        if (eCoordSys::REC == coordSys())
        {
            setRecCoord(x1, x2);
        }
        else if (eCoordSys::POL == coordSys())
        {
            setPolCoord(x1, x2);
        }
    }

    void PVector2D::setRecCoord(phys_float x, phys_float y)
    {
        setX(x);
        setY(y);
    }

    void PVector2D::setPolCoord(phys_float rho, phys_float theta)
    {
        if (rho < 0 && (std::abs(rho) >= PHYS_FLOAT_EPSILON))
        {
            std::cerr << "PVector2D::setPolCoord(rho,theta) error:  "
                      << "you are attempting to assign:    " << rho << " to rho coordinate"
                      << std::endl;

            std::cerr << "rho cannot be negative!" << std::endl;

            std::cerr << "operation aborted!" << std::endl;

            abort();
        }

        setX(std::abs(rho) * std::cos(theta));
        setY(std::abs(rho) * std::sin(theta));
    }

    /***********************************************************************
    *              GETTERS & SETTERS
    ************************************************************************/

    void PVector2D::setRho(phys_float rho)
    {
        setPolCoord(rho, theta());
    }

    void PVector2D::setTheta(phys_float theta)
    {
        setPolCoord(rho(), theta);
    }

    phys_float PVector2D::rho() const
    {
        return sqrt(sqr(x()) + sqr(y()));
    }

    phys_float PVector2D::theta() const
    {
        return std::atan2(y(), x());
    }

    // Get the \f$ e_{\rho} \f$ basis vector in cartesian coordinates
    PVector2D PVector2D::e_rho() const
    {
        return PVector2D(std::cos(theta()), std::sin(theta()), eCoordSys::REC);
    }

    // Get the \f$ e_{\theta} \f$ basis vector in cartesian coordinates
    PVector2D PVector2D::e_theta() const
    {
        return PVector2D(-std::sin(theta()), std::cos(theta()), eCoordSys::REC);
        ;
    }

    /***********************************************************************
    *                   VECTOR ALGEBRA
    ************************************************************************/

    PVector2D PVector2D::operator+(const PVector2D &vectRight) const
    {
        return PVector2D(x() + vectRight.x(), y() + vectRight.y());
    }

    PVector2D PVector2D::operator-(const PVector2D &vectRight) const
    {
        return *this + (-vectRight);
    }

    PVector2D operator-(const PVector2D &vect)
    {
        return PVector2D(-vect.x(), -vect.y());
    }

    phys_float PVector2D::operator*(const PVector2D &vectRight) const
    {
        return x() * vectRight.x() + y() * vectRight.y();
    }

    PVector2D PVector2D::operator*(phys_float number) const
    {
        return PVector2D(number * x(), number * y());
    }

    PVector2D operator*(phys_float number, const PVector2D &vect)
    {
        return vect * number;
    }

    phys_float PVector2D::crossProduct(const PVector2D &vectRight) const
    {
        return x() * vectRight.y() - y() * vectRight.x();
    }

    PVector2D PVector2D::operator/(phys_float divisor) const { return (1 / divisor) * (*this); }

    PVector2D &PVector2D::operator+=(const PVector2D &vectRight)
    {
        *this = *this + vectRight;
        return *this;
    }

    PVector2D &PVector2D::operator-=(const PVector2D &vectRight)
    {
        *this = *this - vectRight;
        return *this;
    }

    PVector2D &PVector2D::operator*=(phys_float number)
    {
        *this = (*this) * number;
        return *this;
    }

    PVector2D &PVector2D::operator/=(phys_float number)
    {
        *this = (*this) / number;
        return *this;
    }

    bool PVector2D::operator==(const PVector2D &vectRight) const
    {
        return (x() == vectRight.x()) && (y() == vectRight.y());
    }

    phys_float PVector2D::cos(const PVector2D &a, const PVector2D &b)
    {
        return (a * b) / (a.norm() * b.norm());
    }

    phys_float PVector2D::angleBetweenVectors(const PVector2D &a, const PVector2D &b)
    {
        auto acos_{std::acos(cos(a, b))};

        return (acos_ > PHYS_FLOAT_ANGLE_EPSILON) ? acos_ : 0;
    }

    /***********************************************************************
    *                   VECTOR MANIPULATION
    ************************************************************************/

    void PVector2D::setVectorAParallelToVectorB(PVector2D &A, const PVector2D &B)
    {
        A.setTheta(B.theta());
    }

    std::ostream &operator<<(std::ostream &os, const PVector2D &vector)
    {
        if (eCoordSys::REC == vector.coordSys())
        {
            os << '(' << vector.x() << ',' << vector.y() << ')';
        }
        else if (eCoordSys::POL == vector.coordSys())
        {
            os << '(' << vector.rho() << ',' << vector.theta() << ')';
        }

        return os;
    }
} // namespace physapi
