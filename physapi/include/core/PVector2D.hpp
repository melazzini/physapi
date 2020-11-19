#pragma once
#include "utils.hpp"

namespace physapi
{
    /**
     * @addtogroup core
     * @{
     *
     */

    /**
     * @brief This class represents a vector in the XY plane.
     *        
     * You can use two different coordinate systems:
     *          - Rectangular(Cartesian): \f$x,\; y\f$.
     *          - Polar : \f$ \rho,\; \theta\f$.
     * 
     * In addition, you can perform all common vector operations(\f$ +, - \f$ ,  etc), 
     * manipulate them and perform with them other operations like printing
     * their components with std::cout, etc.
     */
    class PVector2D
    {
    public:
        /***********************************************************************
        *              CONSTRUCTORS & DESTRUCTOR
        ***********************************************************************/
        /**
         * @brief Construct a new 2D-vector with x=y=0.
         * 
         */
        explicit PVector2D();

        /**
         * @brief Construct a new 2D-vector with the specified coordinates.
         * 
         * As you can see, if you need to create a 2D-vector in rectangular
         * coordinates, then x1 represents x and x2 represents y. On the
         * other hand, if you need a polar vector, then x1 represents \f$ \rho \f$
         * and x2 represents \f$ \theta \f$.
         * 
         * @param x1 x(cartesian) or \f$ \rho \f$(polar)
         * @param x2 y(cartesian) or \f$ \theta \f$(polar)
         * @param coordSys coordinate system
         * 
         * 
         * @note \f$ \rho \f$ cannot be negative!
         */
        PVector2D(phys_float x1,
                  phys_float x2,
                  eCoordSys coordSys = eCoordSys::REC);

        /**
         * @brief Destroy the PVector2D object
         * 
         */
        virtual ~PVector2D() {}

        /***********************************************************************
        *              GETTERS & SETTERS
        ************************************************************************/

        /**
         * @brief Set a new x coordinate.
         * 
         * @param x new x coordinate.
         */
        inline void setX(phys_float x) { m_x = x; }

        /**
         * @brief Set a new y coordinate.
         * 
         * @param y new y coordinate.
         */
        inline void setY(phys_float y) { m_y = y; }

        /**
         * @brief Set a new \f$ \rho \f$ coordinate.
         * 
         * @param rho new \f$ \rho \f$ coordinate.
         * 
         * @note \f$ \rho \f$ cannot be negative!
         */
        void setRho(phys_float rho);

        /**
         * @brief Set a new \f$ \theta \f$ coordinate.
         * 
         * @param theta new \f$ \theta \f$ coordinate.
         */
        void setTheta(phys_float theta);

        /**
         * @brief Set a new coordinate system.
         * 
         * @param newCoordSys new coordinate system.
         */
        inline void setCoordSys(eCoordSys newCoordSys) { m_currentCoordSys = newCoordSys; }

        /**
         * @brief Get the x coordinate.
         * 
         * @return phys_float x coordinate.
         */
        inline phys_float x() const { return m_x; }

        /**
         * @brief Get the y coordinate.
         * 
         * @return phys_float y coordinate.
         */
        inline phys_float y() const { return m_y; }

        /**
         * @brief Get the \f$ \rho \f$ coordinate.
         * 
         * @return phys_float \f$ \rho \f$ coordinate.
         */
        phys_float rho() const;

        /**
         * @brief Get the \f$ \theta \f$ coordinate.
         * 
         * @return phys_float \f$ \theta \f$ coordinate.
         */
        phys_float theta() const;

        /**
         * @brief Get the current coordinate system.
         * 
         * @return eCoordSys current coordinate system.
         */
        inline eCoordSys coordSys() const { return m_currentCoordSys; }

        /**
         * @brief Get the norm of the vector
         * 
         * @return phys_float norm
         */
        virtual phys_float norm() const { return rho(); }

        /**
        * @brief Get the \f$ e_{\rho} \f$ basis vector in cartesian coordinates
        * 
        * @return PVector2D 
        */
        PVector2D e_rho() const;

        /**
        * @brief Get the \f$ e_{\theta} \f$ basis vector in cartesian coordinates 
        * 
        * @return PVector2D 
        */
        PVector2D e_theta() const;

        /**
         * @brief Get the squared of the vector's norm
         * 
         * @return phys_float 
         */
        virtual phys_float normSquared() const { return sqr(PVector2D::norm()); }

        /***********************************************************************
        *                   VECTOR ALGEBRA
        ************************************************************************/

        /**
         * @brief Addition of 2D-vectors.
         * 
         * @param vectRight vector on the right.
         * @return PVector2D result in rectangular coordinates.
         */
        PVector2D operator+(const PVector2D &vectRight) const;

        /**
         * @brief Subtraction of 2D-vectors.
         * 
         * @param vectRight vector on the right.
         * @return PVector2D result
         */
        PVector2D operator-(const PVector2D &vectRight) const;

        /**
         * @brief Get the vector with changed sign.
         * 
         * @param vect vector.
         * @return PVector2D new vector with the opposite sign. 
         */
        friend PVector2D operator-(const PVector2D &vect);

        /**
         * @brief Vector multiplication of 2D-vectors.
         * 
         * @param vectRight vector on the right.
         * @return phys_float result
         */
        phys_float operator*(const PVector2D &vectRight) const;

        /**
         * @brief Multiplication of a 2D-vector by a number.
         * 
         * @param phys_float number on the right.
         * @return PVector2D result.
         */
        PVector2D operator*(phys_float number) const;

        /**
         * @brief Multiplication of a number by a 2D-vector.
         * 
         * @param vectRight vector on the right.
         * @return PVector2D result
         */
        friend PVector2D operator*(phys_float number, const PVector2D &vect);

        /**
         * @brief Floating point value of the cross product of two 2D-vectors on the XY plane.
         * 
         * @param vectRight vector on the right.
         * @return phys_float result.
         */
        phys_float crossProduct(const PVector2D &vectRight) const;

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
        PVector2D operator/(phys_float divisor) const;

        /**
         * @brief Shortcut to add a vector to the original vector.
         * 
         * It adds the two vectors, set the result into the vector on the left
         * and returns a reference to that vector.
         * 
         * @param vectRight vector on the right.
         * @return PVector2D& updated vector(vector on the left) after the addition.
         */
        PVector2D &operator+=(const PVector2D &vectRight);

        /**
         * @brief Shortcut to subtract a vector from the original vector.
         * 
         * It subtracts the two vectors, set the result into the vector on the left
         * and returns a reference to that vector.
         * 
         * @param vectRight vector on the right.
         * @return PVector2D& updated vector(vector on the left) after the subtraction.
         */
        PVector2D &operator-=(const PVector2D &vectRight);

        /**
         * @brief Shortcut to multiply a vector by a number and 
         * get the result into the original vector.
         * 
         * It multiplies the vector  on the left by the number of the right
         * , set the result into the vector 
         * and returns a reference to that vector.
         * 
         * @param number number on the right
         * @return PVector2D& updated vector(vector on the left) after multiplication.
         */
        PVector2D &operator*=(phys_float number);

        /**
         * @brief Shortcut to divide a vector by a number and 
         * get the result into the original vector.
         * 
         * It divides the vector  on the left by the number of the right
         * , set the result into the vector 
         * and returns a reference to that vector.
         * 
         * @param number number on the right
         * @return PVector2D& updated vector(vector on the left) after division.
         */
        PVector2D &operator/=(phys_float number);

        /**
         * @brief Determines if two vectors are equal.
         * 
         * Here "equal" means: \f$ x_{left} = x_{right} \f$ and
         * \f$ y_{left} = y_{right} \f$, i.e. both vectors have the same components.
         * 
         * @param vectRight vector on the right. 
         * @return true - the vectors are equal.
         * @return false - the vectors are not equal. 
         */
        bool operator==(const PVector2D &vectRight) const;

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
        static phys_float cos(const PVector2D &a, const PVector2D &b);

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
        static phys_float angleBetweenVectors(const PVector2D &a, const PVector2D &b);

        /***********************************************************************
        *                   VECTOR MANIPULATION
        ************************************************************************/

        /**
         * @brief Set the Vector A Parallel To Vector B.
         * 
         * @param A vector \f$ \vec{A} \f$.
         * @param B vector \f$ \vec{B} \f$.
         */
        static void setVectorAParallelToVectorB(PVector2D &A, const PVector2D &B);

        /***********************************************************************
        *                   MICELLANEOUSS
        ************************************************************************/

        /**
         * @brief Uses ostream to print the coordinates of the vector.
         * 
         * @param os std::ostream object.
         * @param vect 2D-vector. 
         * @return std::ostream&
         * 
         * @note This function uses the current representation of the vector, rectangular or polar,
         * to print the information. You can specify the behavior changing the representation.
         * 
         * @see setCoordSys() coordSys()
         * 
         */
        friend std::ostream &operator<<(std::ostream &os, const PVector2D &vect);

    protected:
        /**
         * @brief Set new coordinates (x, y) or (\f$ \rho \f$, \f$ \theta \f$).
         * 
         * As you can see, by default you create a vector in rectangular coordinates. 
         *
         * @param x1 x(cartesian) or \f$ \rho \f$(polar).
         * @param x2 y(cartesian) or \f$ \theta \f$(polar).
         * @param coordSys coordinate system.
         *
         * @note \f$ \rho \f$ cannot be negative!
         * 
         * @see PVector2D(phys_float, phys_float, eCoordSys).
         */
        void setCoord(phys_float x1,
                      phys_float x2,
                      eCoordSys coordSys = eCoordSys::REC);

        /**
         * @brief Set the coordinates x,y.
         * 
         * @param x new x coordinate.
         * @param y new y coordinate.
         * 
         * @note This method does not change the internal coordinate representation.
         * 
         * @see setCoordSys()
         */
        void setRecCoord(phys_float x, phys_float y);

        /**
         * @brief Set the coordinates \f$ \rho \f$, \f$ \theta \f$.
         * 
         * @param rho    \f$ \rho \f$ coordinate.
         * @param theta  \f$ \theta \f$ coordinate.
         * 
         * @note \f$ \rho \f$ cannot be negative!
         * 
         * @note This method does not change the internal coordinate representation.
         * 
         * @see setCoordSys()
         */
        void setPolCoord(phys_float rho, phys_float theta);

    private:
        /***********************************************************************
        *              DATA MEMBERS
        ************************************************************************/

        //x coordinate
        phys_float m_x;

        //y coordinate
        phys_float m_y;

        // Current coordinate system representation.
        // We use this to print the vector with std::ostream
        eCoordSys m_currentCoordSys;
    };

    /**
     * @} 
     */

} // namespace physapi
