# 🛺 Cab Route Optimizer — Hyderabad

> A full-stack routing engine simulating real-world cab navigation (Ola/Uber-style) for Hyderabad, powered by a custom C++ Dijkstra implementation, a Node.js REST API, and an interactive map frontend.

---

## Demo

| Feature | Details |
|---|---|
|  Live map | OpenStreetMap + Leaflet.js, no API key needed |
|  Shortest distance | Dijkstra on km-weighted graph |
|  Fastest time | Dijkstra on time-weighted graph with traffic |
|  Peak-hour simulation | 2× time multiplier, auto-detected from travel hour |
|  Route comparison | Both routes drawn simultaneously on map |
| 📍16 Hyderabad locations | Gachibowli → Charminar → LBNagar and more |

---

## Architecture

```
Browser (Leaflet.js)
      │
      │  GET /route?from=X&to=Y&hour=H
      ▼
Node.js / Express  (server.js)
      │
      │  execFile('./cab_route', [from, to, hour])
      ▼
C++ Binary  (main.cpp)
      │
      ├── dijkstra(baseGraph,    source, target, distance)
      └── dijkstra(trafficGraph, source, target, time)
             │
             └── applyTraffic(graph, hour)
                   Peak: 08:00–10:00 AM, 05:00–08:00 PM → 2× multiplier
```

The C++ binary runs **two independent Dijkstra passes** per request and returns a single JSON object. JavaScript never touches the routing logic.

---

## Tech Stack

| Layer | Technology | Why |
|---|---|---|
| Routing engine | C++17 | Performance, direct STL access |
| Graph structure | `unordered_map<string, vector<Edge>>` | O(1) node lookup |
| Priority queue | `std::priority_queue` with `greater<>` | Min-heap for Dijkstra |
| Traffic model | `traffic.cpp` — multiplier on edge weights | Simulates real peak-hour slowdown |
| API server | Node.js + Express | Lightweight, bridges C++ ↔ browser |
| IPC format | JSON over stdout | Clean, parseable, no dependencies |
| Map | Leaflet.js + OpenStreetMap | No API key, production-quality tiles |

---

## Project Structure

```
cab-route-optimizer/
├── graph.h / graph.cpp        # Graph and Edge structs, addRoad()
├── dijkstra.h / dijkstra.cpp  # Dijkstra's algorithm, RouteResult
├── traffic.cpp                # Peak-hour traffic multiplier
├── main.cpp                   # CLI binary — args in, JSON out
├── server.js                  # Express REST API
├── public/
│   └── index.html             # Map UI, dropdowns, comparison table
└── README.md
```

---

## Setup

### Prerequisites
- `g++` with C++17 support
- Node.js v16+

### 1. Compile

```bash
g++ -std=c++17 -O2 -o cab_route main.cpp graph.cpp dijkstra.cpp traffic.cpp
```

### 2. Install dependencies

```bash
npm install express
```

### 3. Run

```bash
node server.js
```

### 4. Open

```
http://localhost:3000
```

---

## API Reference

```
GET /route?from=<location>&to=<location>&hour=<0-23>
```

**Example:**
```
GET /route?from=Gachibowli&to=LBNagar&hour=9
```

**Response:**
```json
{
  "isPeak": true,
  "hour": 9,
  "distance": {
    "totalCost": 24.3,
    "unit": "km",
    "path": ["Gachibowli", "HitechCity", "Madhapur", "Ameerpet", "Begumpet", "LBNagar"]
  },
  "time": {
    "totalCost": 124,
    "unit": "min",
    "path": ["Gachibowli", "HitechCity", "Madhapur", "Ameerpet", "Begumpet", "LBNagar"]
  }
}
```

---

## Algorithm Details

### Dijkstra's Algorithm
- **Complexity:** O((V + E) log V)
- **Data structure:** STL `priority_queue<pair<double, string>, vector<...>, greater<...>>` (min-heap)
- **Graph:** Adjacency list with bidirectional weighted edges
- **Early exit:** Terminates as soon as the target node is popped from the heap
- **Dual-metric edges:** Every edge stores both `distance (km)` and `time (min)`, enabling two independent cost functions on the same graph

### Traffic Simulation
- `traffic.cpp` clones the base graph and scales all edge times by a multiplier
- Peak hours: **08:00–10:00** (morning) and **17:00–20:00** (evening) → `2.0×`
- Off-peak → `1.0×` (no change)
- The hour is extracted from the browser's time picker and passed as a 24-hour integer

---

## Interview Talking Points

- **Why C++ for routing?** Graph traversal is CPU-bound; C++ gives deterministic performance and direct STL control over the heap.
- **Why `execFile` over a native Node addon?** Simpler build, language-agnostic interface, JSON as IPC is clean and testable independently.
- **Why two Dijkstra passes instead of one?** Distance and time are independent cost functions — the shortest-distance path is not always the fastest, especially under traffic. Running both gives the user a real comparison.
- **How does traffic affect the algorithm?** Traffic is modelled at the graph level, not inside the algorithm — `applyTraffic()` returns a modified graph copy with scaled edge weights. This keeps Dijkstra pure and reusable.
- **What would you improve with more time?** Yen's k-shortest paths for alternative routes, persistent graph loading, and OSRM integration for real road geometries.