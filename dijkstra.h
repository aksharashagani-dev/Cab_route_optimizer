#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <string>
#include <vector>
#include "graph.h"
using namespace std;

struct RouteResult {
    vector<string> path;
    double totalCost;
    bool found;
};

RouteResult dijkstra(Graph& g, const string& source,
                      const string& target, bool useTime, bool peakHour);

#endif