#include "dijkstra.h"
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_map>

const double PEAK_HOUR_MULTIPLIER = 1.6;

RouteResult dijkstra(Graph& g, const string& source,
                      const string& target, bool useTime, bool peakHour) {

    unordered_map<string, double> dist;
    unordered_map<string, string> prev;

    for (auto& entry : g.adj) {
        dist[entry.first] = numeric_limits<double>::infinity();
    }
    dist[source] = 0.0;

    priority_queue<pair<double, string>,
                   vector<pair<double, string>>,
                   greater<pair<double, string>>> pq;
    pq.push({0.0, source});

    while (!pq.empty()) {
        double d = pq.top().first;
        string u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        if (u == target) break;

        for (const Edge& e : g.adj[u]) {
            double weight = useTime ? e.time : e.distance;

            if (useTime && peakHour) {
                weight *= PEAK_HOUR_MULTIPLIER;
            }

            double newDist = d + weight;
            if (newDist < dist[e.to]) {
                dist[e.to] = newDist;
                prev[e.to] = u;
                pq.push({newDist, e.to});
            }
        }
    }

    RouteResult result;

    if (dist.find(target) == dist.end() ||
        dist[target] == numeric_limits<double>::infinity()) {
        result.found = false;
        result.totalCost = -1;
        return result;
    }

    result.found = true;
    result.totalCost = dist[target];

    string current = target;
    while (current != source) {
        result.path.push_back(current);
        current = prev[current];
    }
    result.path.push_back(source);
    reverse(result.path.begin(), result.path.end());

    return result;
}