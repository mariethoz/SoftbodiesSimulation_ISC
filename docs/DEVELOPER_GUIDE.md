
## Developer guide

This guide is intended for developers who want to understand, extend, or maintain the soft-body simulation library.

### Architecture Overview

The simulation is structured around a clear separation of responsibilities,
from low-level math utilities to high-level simulation orchestration.

```mermaid
classDiagram
    class Simulation {
        +step(dt)
        +addBody(body)
        +addCollider(collider)
    }

    class SoftBody {
        +applyForce(f)
        +solveConstraint()
        +update(dt)
    }

    class Particle {
        -position
        -prev_position
        -mass
        +applyForce(f)
        +update(dt)
    }

    class Constraint {
        -restLength
        -stiffness
        +applyConstraint()
    }

    class WorldCollider {
        <<abstract>>
        +collide(p)
    }

    class PlaneCollider
    class InnerCircleCollider
    class OuterCircleCollider

    Simulation --> SoftBody
    Simulation --> WorldCollider

    SoftBody --> Particle
    SoftBody --> Constraint

    Constraint --*> Particle

    WorldCollider <|-- PlaneCollider
    WorldCollider <|-- InnerCircleCollider
    WorldCollider <|-- OuterCircleCollider
```

### Design Principles

- `Simulation` orchestrates the full physics pipeline
- `SoftBody` owns particles and constraints
- `Particle` represents a Verlet-integrated mass point
- `Constraint` enforces shape preservation
- `WorldCollider` defines interactions with the environment

### Code structure

```bash
src/
├── cpp/
│   ├── include/    # Simulation public headers
│   └── src/        # Simulation implementation
│       └── main*.cpp
│
├── godot-extension/
│   ├── include/    # Godot GDExtension headers
│   └── src/        # Godot binding implementation
│
├── tests/
│   └── cpp/        # Unit tests for the simulation
│
└── main.cpp
```

**Rule**; The simulation must remain independent from any visualization or game engine.

### Memory Management Rules

Ownership is explicit
- `Simulation` owns:
    - `SoftBody*`
    - `WorldCollider*`
- `SoftBody` owns:
    - `Particle*`
    - `Constraint*`
- Deallocation happens in `Simulation::clear()`

### Doxygen Comments

This project uses Doxygen for API documentation.

```cpp
/**
 * @brief Represents a single particle in the simulation.
 *
 * Particles are integrated using Verlet integration and do not
 * store explicit velocity.
 */
class Particle {
public:
    /**
     * @brief Applies a force to the particle.
     * @param f Force vector
     */
    void applyForce(const Vector2& f);
};
```

Generate documentation;
```bash
 doxygen .\docs\Doxyfile
```
The generated documentation will be available in;
```text
docs/output/html/index.html
```

### Running Tests

You can run Unit Tests after build the **C++ Standalone**.

```bash
cd build
ctest
cd ..
```

### Writing New Tests

- Place the test files in `tests/` following the same logic that for the code.
- Use one test file per class.
- Use **GoogleTest** (`gtest`)

Exemple:
```cpp
TEST(ConstraintTest, AccessorsReturnParticlePositions) {
    Particle p1(Vector2(1, 1));
    Particle p2(Vector2(2, 2));

    Constraint c(&p1, &p2);

    EXPECT_EQ(c.getPart1(), Vector2(1, 1));
    EXPECT_EQ(c.getPart2(), Vector2(2, 2));
}
```