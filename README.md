
# Intelligent Power Grid Restoration System

## 1. Project Overview

This C++ Data Structures micro-project simulates power-grid damage caused by a severe cyclone and recommends an order for repairing damaged power lines.

The system models a simplified virtual grid containing one main power station, four city substations, four city distribution networks, and shared infrastructure.

## 2. Problem Statement

A severe cyclone can damage multiple power lines, leaving hospitals, emergency services, water treatment facilities and residential areas without electricity. Since repairs require time and effort, the order of restoration is important.

This project explores how graph-based algorithms and data structures can help identify outages and prioritize repairs.

## 3. Implemented Prototype Features

- Builds a virtual four-city power grid.
- Generates random power-line damage scenarios.
- Displays damaged lines and estimated repair time and effort.
- Uses BFS to identify locations connected to the power source.
- Calculates repair priorities using facility importance and estimated repair time.
- Uses a priority queue to recommend the next eligible repair.
- Simulates repairs and updates power availability.
- Allows additional line failures and grid resets.
- Supports searching for a power line by ID and sorting damaged lines by repair time.
- Displays a city's tree-like distribution structure.

## 4. Data Structures and Algorithms

| Concept | Use in the project |
|---|---|
| Graph | Represents locations and power-line connections |
| BFS | Checks which locations remain connected to the power source |
| DFS | Included in the proposed design for further connectivity analysis |
| Priority queue | Organizes eligible repair candidates by priority score |
| Greedy approach | Selects a high-priority repair at each step |
| Searching and sorting | Provides line lookup and repair-time ordering |
| Tree-like structure | Represents the branching distribution layout of a city |

## 5. Repair Priority

The prototype calculates a heuristic score:

Priority Score = Potential Facility Priority / Estimated Repair Time

Repair effort is used to break ties between candidates.

The score is a simplified decision-making heuristic. It does not guarantee a globally optimal restoration schedule.

## 6. Repository Structure

- `src/main.cpp` — C++ console application
- `diagrams/power_grid.md` — Virtual grid diagram
- `docs/algorithm_workflow.md` — Restoration workflow
- `tests/test_cases.md` — Planned manual test cases
- `README.md` — Project overview and instructions

## 7. How to Run

A C++17 compiler such as GCC is required.

From the repository root, compile the program:

```bash
g++ -std=c++17 src/main.cpp -o power_grid
```

On Windows PowerShell, run:

```powershell
.\power_grid.exe
```

On Linux or macOS, run:

```bash
./power_grid
```

## 8. Assumptions and Limitations

- The grid is a simplified virtual model, not a verified real electricity network.
- Facility priorities, repair times and repair effort values are illustrative assumptions.
- Repairs are performed one at a time in the console simulation.
- The repair scheduler uses a heuristic and does not guarantee a globally optimal result.
- Multi-crew scheduling and explicit resource-allocation constraints are not implemented.
- The test-case document describes planned checks; each case must be executed before it can be reported as passed.

## 9. Future Scope

- Interactive geographical map and live disaster visualization.
- Automatic damage detection using suitable external data.
- Integration with real-world utility infrastructure data.
- Advanced scheduling for multiple repair crews and limited equipment.
- More detailed modelling of network disruption and restoration performance.

## 10. Technology

- C++17
- Visual Studio Code
- Git and GitHub
- Mermaid diagrams
