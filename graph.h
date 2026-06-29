#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

struct Edge {
    string to;
    double distance;
    double time;
};

struct Graph {
    unordered_map<string, vector<Edge>> adj;

    void addRoad(const string& from, const string& to,
                 double distance, double time, bool bidirectional = true);
};

#endif