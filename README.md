Knowledge Graph Implementation - DSA Assignment 3
Course: CO2003 - Data Structures and Algorithms
University: Ho Chi Minh City University of Technology (HCMUT - VNUHCM)
Semester: 251 (Winter 2025)

**Introduction**
- This project is the implementation of a Knowledge Graph using a Directed Graph data structure. It was developed as Assignment 3 for the Data Structures and Algorithms course.
- The goal of this project is to move beyond simple vector similarity (from previous assignments) and model explicit semantic relationships between entities (e.g., product categories, citations, related concepts).
- The system supports storing entities as vertices, relationships as directed edges, and performing complex graph traversal queries.

**Features**
- DGraphModel<T> (Directed Graph Engine)
A core graph data structure implemented from scratch without using external graph libraries.
Vertex & Edge Management: Add vertices, connect/disconnect edges, and manage weights.
Graph Traversal:
BFS (Breadth-First Search): Traverses the graph layer by layer.
DFS (Depth-First Search): Traverses the graph depth-wise.
Data Integrity: Generic implementation (template <class T>) supporting custom vertex comparison (vertexEQ) and string conversion (vertex2str).
