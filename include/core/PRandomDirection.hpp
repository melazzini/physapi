#pragma once
#include "PVector3D.hpp"

namespace physapi
{
    /**
     * @addtogroup core
     * 
     * @{
     * 
     */

    /**
     * @brief This class generates random directions for vectors.
     * 
     */
    class PRandomDirection
    {
    public:
        /**
         * @brief Construct a new PRandomDirection object.
         * 
         */
        explicit PRandomDirection()
            : m_xi1{}, m_xi2{},
              //we must ensure that this vector always
              //is unitary!!!
              m_vector{1, 0, 0}
        {
        }

        /**
         * @brief Destroy the PRandomDirection object
         * 
         */
        virtual ~PRandomDirection() {}

        /**
         * @brief Set a random direction to a 3d vector
         * 
         * @param vect 3d vector
         */
        void operator()(PVector3D *vect);

        /**
         * @brief Get a random oriented unitary 3d vector
         * 
         * @return const PVector3D& 
         */
        const PVector3D &operator()();

        /**
         * @brief Get a random oriented unitary 3d vector,
         * which satisfies a certain condition, given by
         * a predicate.
         * 
         * The predicate must receive 3d vector as a constant
         * reference parameter.
         * 
         * @tparam T type of the predicate
         * @param predicate predicate
         * @return const PVector3D& random oriente 3d vector according to the predicate 
         */
        template <typename T>
        const PVector3D &operator()(const T &predicate)
        {
            this->operator()(&m_vector);

            while (!predicate(m_vector))
                this->operator()(&m_vector);

            return m_vector;
        }

    private:
        //helper variables to hold random
        //values
        phys_float m_xi1, m_xi2;

        //random direction vector,
        //it must be unitary!!!
        PVector3D m_vector;

        //random numbers generator
        PRandom m_randMgr;
    };

    /**
     * @}
     * 
     */
} // namespace physapi
