#include "PVector3D.hpp"

namespace physapi
{
    PVector3D::PVector3D(phys_float x1,
                         phys_float x2,
                         phys_float x3,
                         eCoordSys coordSys)
    {
        setCoord(x1, x2, x3, coordSys);
    }

    void PVector3D::setCoord(phys_float x1, phys_float x2, phys_float x3, eCoordSys coordSys_)
    {
        setCoordSys(coordSys_);

        if (eCoordSys::REC == coordSys())
        {
            setRecCoord(x1, x2, x3);
        }
        else if (eCoordSys::POL == coordSys())
        {
            setPolCoord(x1, x2, x3);
        }
        else if (eCoordSys::SPH == coordSys())
        {
            setSphCoord(x1, x2, x3);
        }
    }

    void PVector3D::setRecCoord(phys_float x, phys_float y, phys_float z)
    {
        setX(x);
        setY(y);
        setZ(z);
    }

    void PVector3D::setPolCoord(phys_float rho, phys_float theta, phys_float z)
    {
        setRho(rho);
        setTheta(theta);
        setZ(z);
    }

    void PVector3D::setSphCoord(phys_float r, phys_float theta, phys_float phi)
    {
        setRho(std::abs(r * std::cos(phi)));
        setTheta((std::cos(phi) >= 0) ? theta : (theta + Pi));
        setZ(r * std::sin(phi));
    }

    void PVector3D::setPhi(phys_float phi)
    {
        setSphCoord(norm(), theta(), phi);
    }

    void PVector3D::setR(phys_float r)
    {
        setSphCoord(r, theta(), phi());
    }

    PVector3D PVector3D::e_r() const
    {
        PVector3D e_x(1, 0, 0);

        PVector3D e_y(0, 1, 0);

        PVector3D e_z(0, 0, 1);

        auto alpha_x = angleBetweenVectors(e_x, *this);
        auto alpha_y = angleBetweenVectors(e_y, *this);
        auto alpha_z = angleBetweenVectors(e_z, *this);

        return PVector3D(std::cos(alpha_x),
                         std::cos(alpha_y),
                         std::cos(alpha_z));
    }

    PVector3D PVector3D::e_phi() const
    {
        auto e_phi_{e_r().crossProduct(e_theta())};

        e_phi_.setR(1);

        return e_phi_;
    }

    /***********************************************************************
    *                   VECTOR ALGEBRA
    ************************************************************************/
    PVector3D PVector3D::operator+(const PVector3D &vectRight) const
    {
        return PVector3D(x() + vectRight.x(), y() + vectRight.y(), z() + vectRight.z());
    }

    PVector3D PVector3D::operator-(const PVector3D &vectRight) const
    {
        return *this + (-vectRight);
    }

    PVector3D operator-(const PVector3D &vect)
    {
        return PVector3D(-vect.x(), -vect.y(), -vect.z());
    }

    phys_float PVector3D::operator*(const PVector3D &vectRight) const
    {
        return xyComponents() * vectRight.xyComponents() + z() * vectRight.z();
    }

    PVector3D PVector3D::operator*(phys_float number) const
    {
        return PVector3D(x() * number, y() * number, z() * number);
    }

    PVector3D operator*(phys_float number, const PVector3D &vect)
    {
        return vect * number;
    }

    PVector3D PVector3D::crossProduct(const PVector3D &other) const
    {
        return PVector3D(
            y() * other.z() - z() * other.y(),
            z() * other.x() - x() * other.z(),
            xyComponents().crossProduct(other.xyComponents()));
    }

    PVector3D PVector3D::operator/(phys_float divisor) const
    {
        return (*this) * (1 / divisor);
    }

    PVector3D &PVector3D::operator+=(const PVector3D &vectRight)
    {
        return ((*this) = (*this) + vectRight);
    }

    PVector3D &PVector3D::operator-=(const PVector3D &vectRight)
    {
        return ((*this) = (*this) - vectRight);
    }

    PVector3D &PVector3D::operator*=(phys_float number)
    {
        return ((*this) = (*this) * number);
    }

    PVector3D &PVector3D::operator/=(phys_float number)
    {
        return ((*this) = (*this) / number);
    }

    bool PVector3D::operator==(const PVector3D &vectRight) const
    {
        return (x() == vectRight.x() && y() == vectRight.y() && z() == vectRight.z());
    }

    phys_float PVector3D::cos(const PVector3D &a, const PVector3D &b)
    {
        return (a * b) / (a.norm() * b.norm());
    }

    phys_float PVector3D::angleBetweenVectors(const PVector3D &a, const PVector3D &b)
    {
        auto acos_{std::acos(cos(a, b))};

        return (acos_ > PHYS_FLOAT_ANGLE_EPSILON) ? acos_ : 0;
    }

    /***********************************************************************
    *                   VECTOR MANIPULATION
    ************************************************************************/

    /**
    * @brief Set the Vector A Parallel To Vector B.
    * 
    * @param A vector \f$ \vec{A} \f$.
    * @param B vector \f$ \vec{B} \f$.
    */
    static void setVectorAParallelToVectorB(PVector3D &A, const PVector3D &B)
    {
        A.setPhi(B.phi());

        A.setTheta(B.theta());
    }

    /***********************************************************************
    *                   MICELLANEOUSS
    ************************************************************************/

    std::ostream &operator<<(std::ostream &os, const PVector3D &vector)
    {
        if (eCoordSys::REC == vector.coordSys())
        {
            os << '(' << vector.x() << ',' << vector.y() << ',' << vector.z() << ')';
        }
        else if (eCoordSys::POL == vector.coordSys())
        {
            os << '(' << vector.rho() << ',' << vector.theta() << ',' << vector.z() << ')';
        }
        else if (eCoordSys::SPH == vector.coordSys())
        {
            os << '(' << vector.r() << ',' << vector.theta() << ',' << vector.phi() << ')';
        }

        return os;
    }
} // namespace physapi
