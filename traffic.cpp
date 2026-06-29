#include "graph.h"

Graph applyTraffic(Graph& g, int hour) {
    // Morning peak: 8AM–10AM, Evening peak: 5PM–8PM
    bool isPeak = (hour >= 8 && hour <= 10) || (hour >= 17 && hour <= 20);
    double multiplier = isPeak ? 2.0 : 1.0;
    Graph trafficGraph;
    for (auto& x : g.adj) {
        for (Edge& edge : x.second) {
            Edge modified = edge;
            modified.time = edge.time * multiplier;
            trafficGraph.adj[x.first].push_back(modified);
        }
    }
    return trafficGraph;
}