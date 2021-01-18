#pragma once

#include "PVector2D.hpp"

namespace physapi
{
    /**
     * @addtogroup
     * @{
     * 
     */

    /**
     * @addtogroup core
     * 
     * @{
     * 
     */

    /**
     * @brief This class represents a vector in the XYZ space.
     *        
     * You can use three different coordinate systems:
     *          - Rectangular(Cartesian): \f$x,\; y, \; z\f$.
     *          - Polar : \f$ \rho,\; \theta, \; z\f$.
     *          - Spherical: \f$ r,\; \theta, \phi \f$.
     * 
     * In addition, you can perform all common vector operations(\f$ +, - \f$ ,  etc), 
     * manipulate them and perform with them other operations like printing
     * their components with std::cout, etc.
     */
    class PVector3D : public PVector2D
    {
    private:
        // z component
        phys_float m_z;

    public:
        /***********************************************************************
        *              CONSTRUCTORS & DESTRUCTOR
        ***********************************************************************/

        /**
         * @brief Construct a new 3D-vector with x=y=z=0.
         * 
         */
        explicit PVector3D()
            : PVector2D{}, m_z{}
        {
        }

        /**
         * @brief Construct a new 3D-vector with the specified coordinates.
         * 
         * As you can see, if you need to create a 3D-vector in rectangular
         * coordinates, then x1 represents x and x2 represents y, x3 represents z. 
         * On the other hand, if you need a polar vector, then x1 represents \f$ \rho \f$
         * x2 represents \f$ \theta \f$, and x3 represents \f$ \phi \f$.
         * 
         * @param x1 x(cartesian) or \f$ \rho \f$(polar) or r(spherical)
         * @param x2 y(cartesian) or \f$ \theta \f$(polar) or \f$ \theta \f$(spherical)
         * @param x3 z(cartesian) or \f$ z \f$(polar) or \f$ \phi \f$(spherical)
         * 
         * @param coordSys coordinate system
         * 
         * @note neither \f$ \rho \f$ nor r cannot be negative!
         */
        PVector3D(phys_float x1,
                  phys_float x2,
                  phys_float x3,
                  eCoordSys coordSys = eCoordSys::REC);

        /**
         * @brief Construct a new 3D vector from a 2D vector and the z coordinate.
         * 
         * @param vect2D 2D vector.
         * @param z z-cartesian coordinate.
         */
        PVector3D(const PVector2D &vect2D, phys_float z)
            : PVector2D(vect2D), m_z{z}
        {
        }

        /**
         * @brief Destroy the PVector3D object
         * 
         */
        virtual ~PVector3D()
        {
        }

        /***********************************************************************
        *              GETTERS & SETTERS
        ************************************************************************/

        /**
         * @brief Set a new z coordinate.
         * 
         * @param z new z coordinate.
         */
        inline void setZ(phys_float z) { m_z = z; }

        /**
         * @brief Set a new \f$ \phi \f$ coordinate.
         * 
         * @param phi new \f$ \phi \f$ coordinate.
         */
        void setPhi(phys_float phi);

        /**
         * @brief Set the r coordinate, i.e. the norm of the vector.
         * 
         * @return phys_float new r coordinate.
         */
        void setR(phys_float r);

        /**
         * @brief Get the z coordinate.
         * 
         * @return phys_float z coordinate.
         */
        inline phys_float z() const { return m_z; }

        /**
         * @brief Get the r coordinate, i.e. the norm of the vector.
         * 
         * @return phys_float r coordinate.
         */
        phys_float r() const { return norm(); }

        /**
         * @brief Get the \f$ \phi \f$ spherical coordinate.
         * 
         * @return phys_float \f$ \phi \f$ coordinate.
         */
        phys_float phi() const { return std::atan2(m_z, rho()); }

        /**
         * @brief Get the norm of the vector
         * 
         * @return phys_float norm
         */
        phys_float norm() const override { return sqrt(normSquared()); }

        /**
         * @brief Get the squared of the vector's norm
         * 
         * @return phys_float 
         */
        phys_float normSquared() const override { return PVector2D::normSquared() + sqr(m_z); }

        /**
        * @brief Get the \f$ e_{\rho} \f$ basis vector in cartesian coordinates
        * 
        * @return PVector2D 
        */
        PVector3D e_rho() const { return PVector3D(PVector2D::e_rho(), 0); }

        /**
        * @brief Get the \f$ e_{\theta} \f$ basis vector in cartesian coordinates 
        * 
        * @return PVector2D 
        */
        PVector3D e_theta() const { return PVector3D(PVector2D::e_theta(), 0); }

        /**
        * @brief Get the \f$ e_{r} \f$ basis vector in cartesian coordinates 
        * 
        * @return PVector2D 
        */
        PVector3D e_r() const;

        /**
        * @brief Get the \f$ e_{\phi} \f$ basis vector in cartesian coordinates 
        * 
        * @return PVector2D 
        */
        PVector3D e_phi() const;

        /**
         * @brief Get the XY components as a PVector2D
         * 
         * @return PVector2D XY-plane vector component
         */
        PVector2D xyComponents() const { return PVector2D(x(), y()); }

        /***********************************************************************
        *                   VECTOR ALGEBRA
        ************************************************************************/

        /**
         * @brief Addition of 3D-vectors.
         * 
         * @param vectRight vector on the right.
         * @return PVector3D result in rectangular coordinates.
         */
        PVector3D operator+(const PVector3D &vectRight) const;

        /**
         * @brief Subtraction of 3D-vectors.
         * 
         * @param vectRight vector on the right.
         * @return PVector3D result in rectangular coordinates.
         */
        PVector3D operator-(const PVector3D &vectRight) const;

        /**
         * @brief Get the vector with changed sign.
         * 
         * @param vect vector.
         * @return PVector3D new vector with the opposite sign. 
         */
        friend PVector3D operator-(const PVector3D &vect);

        /**
         * @brief Vector multiplication of 3D-vectors.
         * 
         * @param vectRight vector on the right.
         * @return phys_float result
         */
        phys_float operator*(const PVector3D &vectRight) const;

        /**
         * @brief Multiplication of a 3D-vector by a number.
         * 
         * @param phys_float number on the right.
         * @return PVector3D result.
         */
        PVector3D operator*(phys_float number) const;

        /**
         * @brief Multiplication of a number by a 3D-vector.
         * 
         * @param vectRight vector on the right.
         * @return PVector3D result
         */
        friend PVector3D operator*(phys_float number, const PVector3D &vect);

        /**
         * @brief Calculates the cross product of two 3d vectors
         * 
         * @param other right 3d vector
         * @return PVector3D cross product
         */
        PVector3D crossProduct(const PVector3D &other) const;

        /**
         * @brief Get the a new vector divided by the given number.
         *      
         * \f$ Result = \frac{\vec{v}}{divisor} = \left(\frac{1}{divisor}\right) * \vec{v} \f$
         * 
         * @param divisor number which divides the vector.
         *       
         * 
         * @return PVector2D Result(see the formula above).
         */
        PVector3D operator/(phys_float divisor) const;

        /**
         * @brief Shortcut to add a vector to the original vector.
         * 
         * It adds the two vectors, set the result into the vector on the left
         * and returns a reference to that vector.
         * 
         * @param vectRight vector on the right.
         * @return PVector3D& updated vector(vector on the left) after the addition.
         */
        PVector3D &operator+=(const PVector3D &vectRight);

        /**
         * @brief Shortcut to subtract a vector from the original vector.
         * 
         * It subtracts the two vectors, set the result into the vector on the left
         * and returns a reference to that vector.
         * 
         * @param vectRight vector on the right.
         * @return PVector3D& updated vector(vector on the left) after the subtraction.
         */
        PVector3D &operator-=(const PVector3D &vectRight);

        /**
         * @brief Shortcut to multiply a vector by a number and 
         * get the result into the original vector.
         * 
         * It multiplies the vector  on the left by the number of the right
         * , set the result into the vector 
         * and returns a reference to that vector.
         * 
         * @param number number on the right
         * @return PVector3D& updated vector(vector on the left) after multiplication.
         */
        PVector3D &operator*=(phys_float number);

        /**
         * @brief Shortcut to divide a vector by a number and 
         * get the result into the original vector.
         * 
         * It divides the vector  on the left by the number of the right
         * , set the result into the vector 
         * and returns a reference to that vector.
         * 
         * @param number number on the right
         * @return PVector3D& updated vector(vector on the left) after division.
         */
        PVector3D &operator/=(phys_float number);

        /**
         * @brief Determines if two vectors are equal.
         * 
         * Here "equal" means: \f$ x_{left} = x_{right} \f$,
         * \f$ y_{left} = y_{right} \f$, and \f$ z_left{} = z_{right} $ 
         * i.e. both vectors have the same components.
         * 
         * @param vectRight vector on the right. 
         * @return true - the vectors are equal.
         * @return false - the vectors are not equal.
         */
        bool operator==(const PVector3D &vectRight) const;

        /**
         * @brief Determines if two vectors are not equal.
         *  
         * @see operator==()
         * 
         * @param vectRight 
         * @return true 
         * @return false 
         */
        bool operator!=(const PVector3D &vectRight) const { return !(*this == vectRight); }

        /**
         * @brief Calculates the cosine of the angle between two vectors.
         * 
         * \f$ \cos( \phi ) = \frac{ \vec{a} * \vec{b} }{ | \vec{a} | * | \vec{b} | } \f$
         * 
         * @param a vector \f$ \vec{a} \f$
         * @param b vector \f$ \vec{b} \f$
         * 
         * @return phys_float cosine of the angle between the vectors: \f$ \cos( \phi )\f$
         */
        static phys_float cos(const PVector3D &a, const PVector3D &b);

        /**
         * @brief Calculates the angle \f$ \phi \f$ between two vectors.
         * 
         * Here \f$ \phi \f$ is given by:
         * \f$ \cos( \phi ) = \frac{ \vec{a} * \vec{b} }{ | \vec{a} | * | \vec{b} | } \f$
         * 
         * @param a vector \f$ \vec{a} \f$
         * @param b vector \f$ \vec{b} \f$
         * 
         * @return phys_float angle \f$ \phi \f$ between the vectors.
         */
        static phys_float angleBetweenVectors(const PVector3D &a, const PVector3D &b);

        /**
         * @brief Return the components of the vector
         * 
         * @param index 0-x, 1-y, 2-z
         * @return phys_float 
         */
        phys_float operator[](size_t index)
        {
            if (index == 0)
            {
                return x();
            }
            else if (index == 1)
            {
                return y();
            }
            else
            {
                return z();
            }
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
        static void setVectorAParallelToVectorB(PVector3D &A, const PVector3D &B);

        /***********************************************************************
        *                   MICELLANEOUSS
        ************************************************************************/

        /**
         * @brief Uses ostream to print the coordinates of the vector.
         * 
         * @param os std::ostream object.
         * @param vect 3D-vector. 
         * @return std::ostream&
         * 
         * @note This function uses the current representation of the vector, rectangular, polar or spherical,
         * to print the information. You can specify the behavior changing the representation.
         * 
         * @see setCoordSys() coordSys()
         * 
         */
        friend std::ostream &operator<<(std::ostream &os, const PVector3D &vect);

    protected:
        void setCoord(phys_float x1, phys_float x2, phys_float x3, eCoordSys coordSys);

        void setRecCoord(phys_float x, phys_float y, phys_float z);

        void setPolCoord(phys_float rho, phys_float theta, phys_float z);

        void setSphCoord(phys_float r, phys_float theta, phys_float phi);
    };

    /**
     * @}
     * 
     */
} // namespace physapi
