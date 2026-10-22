@page glossary Project Glossary

# Project Glossary: Domain, Architecture & Algorithms

This glossary defines the core concepts, data structures, and architectural principles applied in the Galaxy Simulator project, particularly highlighting the Object-Oriented Programming (OOP) refactoring efforts.

## 1. Domain Entities
* **Celestial Object** - The base abstract data type representing any physical entity in the simulation. It defines the common interface for interaction.
* **Galaxy** - The primary domain container. It manages the collection of celestial bodies and their relationships (distances) using a graph data structure.
* **Star System** - A composite celestial body containing a central star and a collection of orbiting planets bound by gravity.
* **Planet / Star / Nebula** - Concrete implementations of celestial bodies with unique physical characteristics.
* **RGBColor** - A UI-independent data structure designed to store color and opacity (alpha channel) purely at the business-logic level, decoupling the domain from the Qt framework.

## 2. Data Structures & Algorithms
* **Graph** - A mathematical abstraction consisting of vertices (representing celestial objects) and weighted edges (representing interstellar routes and distances).
* **Adjacency List / Adjacency Matrix** - Two distinct internal representations of the graph. They optimize memory usage and algorithm performance depending on the density of the universe's connections.
* **Dijkstra's Algorithm** - A shortest-path graph algorithm utilized to calculate the optimal travel route and minimum distance between any two star systems.
* **BFS (Breadth-First Search) / DFS (Depth-First Search)** - Classic graph traversal algorithms used to explore the universe's topology and verify network connectivity.

## 3. Architecture & Design Patterns (OOP)
* **Strategy Pattern** - A behavioral design pattern that extracts the logic of each graph traversal algorithm (BFS, DFS, Dijkstra) into separate, interchangeable classes. This allows dynamic algorithm selection at runtime.
* **Factory Method Pattern** - A creational pattern (implemented via `GalaxyFactory`) that encapsulates the complex object creation logic, including JSON parsing and randomization. It frees domain classes from creation responsibilities.
* **SRP (Single Responsibility Principle)** - Achieved by ensuring that domain classes (`Planet`, `Galaxy`) only hold data, the `GalaxyFactory` creates them, and the UI (`GraphWidget`) renders them.
* **OCP (Open/Closed Principle)** - Realized by extracting the `GraphAlgorithm` interface. The system is now open for extension but closed for modification.
* **God Object** - An anti-pattern successfully eliminated during refactoring by splitting the legacy, monolithic `Algorithms` class into independent strategy classes.

## 4. Technology Stack
* **Qt Framework** - A comprehensive C++ library used for building the Graphical User Interface (GUI) and handling custom 2D rendering of the galaxy.
* **Bullet Physics** - An external physics engine integrated to calculate gravitational fields, mass interactions, and simulate rigid body dynamics in 3D space.
* **JSON (JavaScript Object Notation)** - A lightweight data-interchange format used for flexible, external configuration of procedural universe generation parameters (e.g., planet spawn probabilities, star types).