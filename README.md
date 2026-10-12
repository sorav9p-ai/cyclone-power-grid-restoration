
# Intelligent Power Grid Restoration System

## 1. Project Overview

This Data Structures micro-project aims to develop a C++ decision-support system that simulates power-grid damage caused by a severe cyclone and recommends an effective repair order.

The system represents a simplified power grid as a graph containing power stations, substations, cities, facilities and connecting power lines. It prioritizes the restoration of critical services while considering repair time, effort and network connectivity.

## 2. Problem Statement

A severe cyclone can damage multiple power lines and substations, causing hospitals, water treatment facilities, communication centres and residential areas to lose electricity. Since repair teams and equipment are limited, the order of repairs can significantly affect how quickly essential services are restored.

Our project uses graph-based algorithms and data structures to analyse damage and recommend suitable repair priorities.

## 3. Objectives

- Represent the power grid using a graph.
- Simulate cyclone damage to power lines.
- Identify facilities disconnected from the power source.
- Prioritize repairs based on criticality, repair time and effort.
- Update the network after repairs or additional failures.
- Demonstrate the system through a C++ console application.

## 4. Data Structures and Algorithms

| Concept | Purpose |
|---|---|
| Graph | Represents the power grid and its connections |
| BFS | Finds locations connected to the power source |
| DFS | Explores connected locations and distribution branches |
| Priority Queue | Organizes repair tasks by calculated priority |
| Greedy Algorithm | Selects a high-benefit repair at each step |
| Searching and Sorting | Finds and organizes repair information |
| Tree | Represents branching distribution structures |

## 5. Proposed Workflow

1. Create the normal power grid.
2. Generate a cyclone-damage scenario.
3. Identify damaged lines and affected facilities.
4. Analyse connectivity using BFS/DFS.
5. Calculate repair priorities using service importance, time and effort.
6. Recommend the next repair using a priority queue.
7. Update the network and recalculate after repairs or new failures.

## 6. Project Scope

The initial system will use a simplified virtual grid and simulated data. Repair times, effort values and facility priorities are illustrative assumptions, not official utility data.

The system is an educational prototype and is not intended to control a real electricity network.

## 7. Future Scope

Future improvements may include an interactive geographical map, live disaster visualization, automatic damage detection, integration with real-world utility data and advanced scheduling for multiple repair crews.

## 8. Technology

- Programming Language: C++
- Development Environment: Visual Studio Code
- Version Control: Git and GitHub
- Diagrams: Mermaid and Excalidraw

## 9. Project Status

The project structure and algorithmic design are being developed. Implementation and testing will be completed incrementally.
