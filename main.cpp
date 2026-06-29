#include <iostream>
#include <string>
#include "graph.h"
#include "dijkstra.h"
using namespace std;

void buildCityNetwork(Graph& g);

// traffic.cpp signature
Graph applyTraffic(Graph& g, int hour);

void printRouteJSON(const RouteResult& r, const string& mode, bool peak) {
    cout << "{";
    cout << "\"found\":true";
    cout << ",\"mode\":\"" << mode << "\"";
    cout << ",\"peakHour\":" << (peak ? "true" : "false");
    cout << ",\"totalCost\":" << r.totalCost;
    cout << ",\"unit\":\"" << (mode == "time" ? "min" : "km") << "\"";
    cout << ",\"path\":[";
    for (size_t i = 0; i < r.path.size(); i++) {
        cout << "\"" << r.path[i] << "\"";
        if (i + 1 < r.path.size()) cout << ",";
    }
    cout << "]}";
}

int main(int argc, char* argv[]) {
    // Usage: cab_route <source> <destination> <hour 0-23>
    if (argc < 4) {
        cout << "{\"error\":\"Usage: cab_route <source> <destination> <hour>\"}" << endl;
        return 1;
    }

    string source = argv[1];
    string target = argv[2];
    int hour      = stoi(argv[3]);

    Graph base;
    buildCityNetwork(base);

    if (base.adj.find(source) == base.adj.end()) {
        cout << "{\"error\":\"Unknown source: " << source << "\"}" << endl;
        return 1;
    }
    if (base.adj.find(target) == base.adj.end()) {
        cout << "{\"error\":\"Unknown destination: " << target << "\"}" << endl;
        return 1;
    }

    bool isPeak = (hour >= 8 && hour <= 10) || (hour >= 17 && hour <= 20);

    // Apply traffic to a copy for time-based routing
    Graph trafficGraph = applyTraffic(base, hour);

    RouteResult distResult = dijkstra(base,         source, target, false, false);
    RouteResult timeResult = dijkstra(trafficGraph, source, target, true,  false);

    if (!distResult.found || !timeResult.found) {
        cout << "{\"error\":\"No route found\"}" << endl;
        return 1;
    }

    // Output both results in one JSON object
    cout << "{";
    cout << "\"isPeak\":" << (isPeak ? "true" : "false");
    cout << ",\"hour\":" << hour;
    cout << ",\"distance\":";
    printRouteJSON(distResult, "distance", false);
    cout << ",\"time\":";
    printRouteJSON(timeResult, "time", isPeak);
    cout << "}" << endl;

    return 0;
}

void buildCityNetwork(Graph& g) {
    // Original roads
    g.addRoad("Gachibowli",    "HitechCity",     3.2,  8.0);
    g.addRoad("HitechCity",    "Madhapur",       2.1,  6.0);
    g.addRoad("Madhapur",      "JubileeHills",   4.5, 12.0);
    g.addRoad("JubileeHills",  "BanjaraHills",   3.0,  9.0);
    g.addRoad("BanjaraHills",  "Ameerpet",       2.8,  7.0);
    g.addRoad("Ameerpet",      "Begumpet",       3.5, 10.0);
    g.addRoad("Begumpet",      "Secunderabad",   4.0, 11.0);
    g.addRoad("Ameerpet",      "Kukatpally",     6.0, 15.0);
    g.addRoad("Gachibowli",    "Kukatpally",     9.5, 22.0);
    g.addRoad("Madhapur",      "Ameerpet",       5.0, 13.0);
    g.addRoad("Secunderabad",  "LBNagar",       12.0, 28.0);
    g.addRoad("Begumpet",      "LBNagar",       10.5, 25.0);
    // New nodes
    g.addRoad("Secunderabad",  "MGBS",           5.5, 14.0);
    g.addRoad("MGBS",          "Charminar",      2.0,  7.0);
    g.addRoad("Charminar",     "Dilsukhnagar",   6.0, 16.0);
    g.addRoad("Dilsukhnagar",  "LBNagar",        4.5, 12.0);
    g.addRoad("LBNagar",       "Uppal",          5.0, 13.0);
    g.addRoad("Uppal",         "Secunderabad",   8.0, 20.0);
    g.addRoad("Kukatpally",    "KPHB",           3.0,  8.0);
    g.addRoad("KPHB",          "Miyapur",        4.0, 10.0);
    g.addRoad("Miyapur",       "Gachibowli",     8.5, 20.0);
    g.addRoad("BanjaraHills",  "Charminar",      7.0, 18.0);
    g.addRoad("Ameerpet",      "MGBS",           6.5, 17.0);
}