#include "graph.h"

void Graph::addRoad(const string& from, const string& to,
                     double distance, double time, bool bidirectional) {
    adj[from].push_back(Edge{to, distance, time});

    if (adj.find(to) == adj.end()) {
        adj[to];
    }

    if (bidirectional) {
        adj[to].push_back(Edge{from, distance, time});
    }
}