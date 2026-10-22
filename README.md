# Galaxy Simulation (OOP Refactoring Project)

**Galaxy Simulation** is a comprehensive C++ project that models the structure and dynamics of a galaxy using graph theory. The project combines structural modeling, physics simulation (Bullet Physics), and interactive 3D visualization (Qt). Each astronomical object (star, planet, nebula) is represented as a vertex in a graph, while cosmic connections are modeled as edges with dynamic weights.

This repository serves as a showcase of advanced **Object-Oriented Programming (OOP) principles, Design Patterns, and Modular Architecture**.

---

## Recent Architectural Refactoring

The codebase recently underwent a massive structural refactoring to eliminate technical debt (specifically, a monolithic God Object anti-pattern) and adhere strictly to **SOLID** principles:

* **Strategy Pattern (OCP & DIP):** Graph traversal and pathfinding algorithms (BFS, DFS, Dijkstra) were decoupled from a monolithic class into independent, highly cohesive strategy classes implementing a common `GraphAlgorithm` interface.
* **Factory Method (SRP):** The creation and randomization of celestial bodies were extracted from the main domain model into a dedicated `GalaxyFactory`.
* **Domain Decoupling:** UI dependencies (like `QColor` from Qt) were completely removed from the domain layer (e.g., `Planet` class) and replaced with abstract data structures (`RGBColor`).
* **Component-Based Modularization:** The build system (CMake) was entirely rewritten to separate the project into independent logical components.

---

## Project Architecture (CMake Components)

The system is strictly divided into independent targets to ensure maximum reusability and separation of concerns:

1. **`GalaxyEngine` (Static Library):** The core business logic. Contains pure mathematical graph structures, domain entities (`Star`, `Planet`), pathfinding strategies, and physics simulation. Completely isolated from the UI.
2. **`Project1` (UI Executable):** The graphical presentation layer built with Qt6 (Widgets & QML). It handles 2D/3D rendering and user interaction, linking dynamically to the `GalaxyEngine`.
3. **`AllTests` (Executable):** A standalone GoogleTest suite verifying the mathematical correctness of the graph and algorithms.
4. **`Benchmark` (Executable):** A dedicated profiling tool using `std::chrono` to measure the performance impact of virtual method dispatching in the new Strategy-based architecture.

---

## Documentation & Reports

Detailed technical documentation and architectural analysis can be found in the following documents:

*  **[OOP Refactoring & Performance Analysis](https://sofiialashyna.github.io/OOP_Lab1_2026/refactoring.html):** A deep dive into the SOLID principles applied, LOC (Lines of Code) metric analysis, and execution speed benchmarks (Legacy vs. Strategy Pattern). Includes comprehensive **UML Diagrams** (Class, Component, Sequence, Communication).
*  **[PlantUML Retrospective Report](https://docs.google.com/document/d/1Uq_FQ6dYK-oBOu9U_GKkszkqOicwvmeiMF0Hy7Cwq4U/edit?usp=sharing):** An analysis of the Docs-as-Code modeling approach using PlantUML for architectural visualization.
*  **[Doxygen Code Documentation](https://sofiialashyna.github.io/OOP_Lab1_2026/):** Auto-generated API documentation detailing the classes, interfaces, and methods (Hosted on GitHub Pages).
*  **[Project Glossary](https://sofiialashyna.github.io/OOP_Lab1_2026/glossary.html):** Defines the core domain terminology used throughout the codebase.

---

## Key Features

1. **Advanced Visualization & UI**
    * **2D Graph View:** Interactive node-link diagram using `QPainter`. Supports zooming, panning, and object inspection.
    * **3D Space View:** Implemented using Qt Quick & QML. Realistic rendering with texture mapping and dynamic lighting.
2. **Physics Engine Integration**
    * **Bullet Physics SDK:** Custom central Black Hole gravity field and spring dynamics for graph edges.
3. **Graph Algorithms**
    * Strategy-based implementations of **BFS**, **DFS**, and **Dijkstra's Algorithm** for real-time pathfinding visualization.

---

## Technologies Used

* **Language:** C++20
* **Build System:** CMake
* **Framework:** Qt 6 (QtWidgets, QtQuick, Quick3D)
* **External Libraries:** * Bullet Physics SDK (Rigid body dynamics)
    * nlohmann/json (Serialization)
* **Testing & Profiling:** GoogleTest (gtest), `std::chrono`
* **Modeling & Docs:** PlantUML, Doxygen, Markdown
* **Plugins:** Statistic (for automated Lines of Code (LOC) metrics and cohesion analysis)

---
*Developed as part of the Object-Oriented Programming Course.*