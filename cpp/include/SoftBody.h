#pragma once
#include <vector>
#include <nlohmann/json.hpp>

#include "Particle.h"
#include "Constraint.h"
#include "Vector2.h"

using json = nlohmann::json;

namespace sim {
    /**
     * @brief Represents a soft body composed of particles and constraints.
     * 
     * A SoftBody is made up of multiple Particles connected by Constraints,
     * allowing it to simulate deformable objects. It supports applying forces,
     * solving constraints, and updating the state of the soft body over time.
     */
    class SoftBody {
    public:
        /**
         * @brief Creates a SoftBody from a polygon by generating a mesh of particles and constraints.
         * 
         * The polygon is meshed into particles and constraints forming a deformable body.
         * 
         * @param polygon The vertices of the polygon defining the soft body's shape.
         * @param mesh_unit The distance between particles in the mesh.
         * @param mass The mass of each particle.
         * @param radius The radius of each particle.
         * @param stiffness The stiffness of the constraints connecting the particles.
         * @param damping The damping factor for the constraints.
         * @param friction The friction coefficient of the soft body.
         * @param restitution The restitution (bounciness) coefficient of the soft body.
         * @param is_pinned Whether the border particles should be pinned (immovable).
         * @return SoftBody* A pointer to the newly created SoftBody instance.
         */
        static SoftBody* createFromPolygon(
            const std::vector<Vector2>& polygon,
            int mesh_unit = 10,
            double mass = 1,
            double radius = 1,
            double stiffness = 0.8,
            double damping = 0.1,
            double friction = 0.1,
            double restitution = 0.9,
            bool is_pinned = false
        );

        SoftBody(
            std::vector<Particle*> particles,
            std::vector<Constraint*> constraints = std::vector<Constraint*> {},
            double friction = 0.1, double restitution = 0.9
        );

        SoftBody(
            std::vector<Particle*> border,
            std::vector<Particle*> particles,
            std::vector<Constraint*> constraints,
            double friction = 0.1, double restitution = 0.9, int unit = 10
        );

        ~SoftBody();

        // Core function
        /**
         * @brief Applies a force to all particles in the soft body.
         * @param f The force vector to apply.
         */
        void applyForce(const Vector2& f);

        /**
         * @brief Solves all constraints in the soft body to maintain its structure.
         */
        void solveConstraint();

        /**
         * @brief Updates the state of the soft body over a time step.
         * @param dt The time step duration.
         */
        void update(double dt);

        // --- Accessors & mutators ----
        std::vector<Particle*> getParticles() { return particles; }
        std::vector<Particle*> getBorder() { return border; }
        std::vector<Constraint*> getConstraints() { return constraints; }
        double getFriction() { return friction; }
        double getRestitution() { return restitution; }

        // --- Saver & Loader ----
        json as_json();
        static SoftBody* from_json(json data);

    protected:
        std::vector<Particle*> particles;       /// Particles making up the soft body
        std::vector<Particle*> border;          /// Border particles of the soft body
        std::vector<Constraint*> constraints;   /// Constraints connecting the particles
        double friction;                        /// Friction coefficient of the soft body [smooth 0 < 1 rough]
        double restitution;                     /// Restitution (bounciness) coefficient of the soft body [sticky 0 < 1 reflect]
        int mesh_unit;                          /// Distance between particles in the mesh
    };
    

    // ---------------------------------------------------------------------------
    // Divide a segment into n equal subdivisions
    // ---------------------------------------------------------------------------
    
    /**
     * @brief Divides a line segment between two points into n equal subdivisions.
     * 
     * The function returns a vector of points that divide the segment from P1 to P2 included.
     * 
     * @param P1 The starting point of the segment.
     * @param P2 The ending point of the segment.
     * @param n The number of subdivisions.
     * @return std::vector<Vector2> A vector containing the points dividing the segment.
     */
    static std::vector<Vector2> divideSegment(const Vector2& P1, const Vector2& P2, int n) {
        std::vector<Vector2> out;
        if (P1 == P2) {
            out.push_back(P1);
            return out;
        }
        if (n <= 0) {
            out.push_back(P1);
            out.push_back(P2);
            return out;
        }

        for (int i = 0; i <= n; ++i) {
            double t = double(i) / double(n);
            out.push_back(interpolate(P1, P2, t));
        }
        return out;
    }
}