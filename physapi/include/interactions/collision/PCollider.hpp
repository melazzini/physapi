#pragma once
#include "utils.hpp"

namespace physapi
{
    /**
     * @brief This class performs a classical collision between two particles
     * 
     * @tparam T_objA type of particle A
     * @tparam T_objB type of particle B
     * @tparam T_collision type of collision
     * 
     * @ingroup collision
     */
    template <typename T_objA, typename T_objB, typename T_collision>
    class PCollider
    {

    public:
        /**
         * @brief Construct a new PCollider object
         * 
         */
        explicit PCollider() {}

        /**
         * @brief Destroy the PCollider object
         * 
         */
        virtual ~PCollider() {}

        /**
         * @brief Perform the collision.
         * 
         * @param particleA particle A
         * @param particleB particle B
         * 
         * @note This method will set the position of particle B to
         * the position of particle A
         */
        virtual void run(T_objA &particleA, T_objB &particleB) const
        {
            particleB.setPosition(particleA.position());

            T_collision collisionHnd{particleA, particleB};

            particleA.setMomentum(collisionHnd.momentumA());

            particleB.setMomentum(collisionHnd.momentumB());
        }
    };
} // namespace physapi
