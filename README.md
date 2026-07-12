# Bus Transit & Ticket Management System

A high-performance C++ console application designed to model, optimize, and manage a municipal bus transportation network. The system leverages an in-memory **Graph Data Structure** alongside **Dijkstra’s Algorithm** to calculate optimal commuter paths based on distance and fare configurations, paired with flat-file CSV handling for transactional data persistence.

---

## 🚀 Key Features

*   **Graph-Modeled Transit Network:** Represents transit hubs as nodes (vertices) and individual bus routes as directed, weighted edges (weights mapping to distance and pricing scales).
*   **Intelligent Routing Engine (Dijkstra's):** Employs a min-priority queue (min-heap) to evaluate branching connections, bottlenecks, and alternative paths, tracking parent vertices to trace routes dynamically.
*   **Sequential Travel Itineraries:** Generates detailed step-by-step ticket breakdowns for multi-stop transfers, letting commuters know exactly where to change buses and the cost of each individual leg.
*   **Dual-Role Access Control:** 
    *   *Admin Portal:* Protected by secure ID/Password lookups; allows real-time expansion of network networks (adding stops/routes) with collision-safeguards to prevent duplicate IDs.
    *   *Passenger Portal:* Allows users to search hubs, calculate shortest paths, book tickets, or cancel reservations instantly.
*   **Flat-File Data Persistence:** Implements robust file handling (`fstream`) to load and sync network modifications and ticket ledger status across application lifecycles.

---

## 🛠️ System Architecture

The project is structured modularly following strict Object-Oriented Programming (OOP) principles to isolate business logic, data structures, and state management:

*   `Models.h`: Contains core data models (`stop`, `Route`, `Ticket`) using encapsulated variables, initialization lists, and explicit constructors.
*   `Graph.h`: Manages the adjacency list maps, stop search lookups, network loaders, and the implementation of Dijkstra's short-path algorithm.
*   `TicketSystem.h`: Coordinates seat booking transaction rules, sequential leg generators, and raw record stream parsing.
*   `main.cpp`: Orchestrates the runtime application loops, structural console menus, and file initializations.

---
## Sample Ticket 
'=========================================='
       TICKET BOOKED SUCCESSFULLY!        
'=========================================='
TICKET ID      : 1
PASSENGER NAME : Pranav
TOTAL DISTANCE : 6.2 km
TOTAL FARE     : 30 Rs

TRAVEL ITINERARY ROUTE BREAKDOWN:
1) VNIT Campus to Ramdaspeth [Fare: 12 Rs]
2) Ramdaspeth to Nagpur Junction [Fare: 18 Rs]
'=========================================='

## 📦 Sample Dataset & Test Cases

The application includes an extensive pre-configured city route grid to test branching loops, one-way directed limits, and dead ends:

### Visualizing the Test Network Matrix:
*   Going **VNIT Campus ➔ Nagpur Junction** has two path choices:
    *   *Path A:* Via Sitabuldi Square = `7.5 km | 35 Rs`
    *   *Path B:* Via Ramdaspeth = `6.2 km | 30 Rs` (The system correctly selects this option)
*   Going **Nagpur Junction ➔ VNIT Campus** tests one-way directed graph constraints. Because roads are mapped as directed legs, it forces a safe reroute around the grid via Sadar Bazaar.

---

## 💻 Getting Started

### Prerequisites
*   A C++ compiler supporting standard **C++11** or higher (`g++` recommended).

### Compilation
Open your terminal inside the project directory and run:
```bash
g++ main.cpp -o BusTransitSystem