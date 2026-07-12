#pragma once
#include "Models.h"
#include <map>
#include <vector>
#include <unordered_map>
#include <queue>
#include <tuple>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <climits>
using namespace std;

class graph {
private:
    map<int, stop> stops;
    map<int, vector<Route>> adjlist;
    unordered_map<int, float> dist;
    unordered_map<int, int> fare;
    unordered_map<int, int> parent;

public:
    void AddStop(stop s) {
        stops.insert({s.getStopID(), s});
    }
    
    void AddRoute(Route r) {
        int SourceID = r.getSource().getStopID();
        adjlist[SourceID].push_back(r);
    }

    stop* SearchStopbyID(int id) {
        for (auto &r : stops) {
            if (r.first == id) return &(r.second);
        }
        return nullptr;
    }

    string tolowercase(string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    stop* SearchStopbyName(string name) {
        string searchname = tolowercase(name);
        for (auto &r : stops) {
            if (tolowercase(r.second.getStopName()) == searchname) return &(r.second);
        }
        return nullptr;
    }

    tuple<vector<int>, float, int> ShortestPath(int StartID, int EndID) {
        if (stops.find(StartID) == stops.end() || stops.find(EndID) == stops.end()) {
            cout << "One of the ID's not found.\n";
            return make_tuple(vector<int>(), -1.0f, -1);
        }

        for (auto &b : stops) {
            dist[b.first] = INT_MAX;
            fare[b.first] = INT_MAX;
            parent[b.first] = -1;
        }
        dist[StartID] = 0;
        fare[StartID] = 0;

        priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int> > > pq;
        pq.push({0, StartID});

        while (!pq.empty()) {
            pair<float, int> topNode = pq.top();
            float CurrDist = topNode.first;
            int u = topNode.second;
            pq.pop();

            if (CurrDist > dist[u]) continue;
            for (auto &edge : adjlist[u]) {
                int v = edge.getDestination().getStopID();
                float weight = edge.getDistance();
                int farex = edge.getFare();
                if (weight + dist[u] < dist[v]) {
                    dist[v] = weight + dist[u];
                    fare[v] = farex + fare[u];
                    pq.push({dist[v], v});
                    parent[v] = u;
                }
            }
        }

        if (dist[EndID] == INT_MAX) {
            cout << "No path exists between the given stops!" << endl;
            return make_tuple(vector<int>(), -1.0f, -1);
        }

        vector<int> path;
        for (int i = EndID; i != -1; i = parent[i]) {
            path.push_back(i);
        }
        reverse(path.begin(), path.end());
        return make_tuple(path, dist[EndID], fare[EndID]);
    }

    void PrintShortestPath(int StartID, int EndID) {
        tuple<vector<int>, float, int> result = ShortestPath(StartID, EndID);
        vector<int> path = get<0>(result);
        float totaldist = get<1>(result);
        int totalfare = get<2>(result);

        if (path.empty()) return;

        for (size_t i = 0; i < path.size(); i++) {
            stop *s = SearchStopbyID(path[i]);
            if (s) {
                cout << s->getStopName();
                if (i < path.size() - 1) cout << " -> ";
            }
        }
        cout << "\nTotal Distance: " << totaldist << " km\nTotal Fare: " << totalfare << " Rs\n";
    }

    void LoadStopInCSV(const string &filename) {
        ifstream read(filename);
        if (!read.is_open()) return;
        string line;
        getline(read, line); 
        while (getline(read, line)) {
            stringstream ss(line);
            string id, stopname;
            getline(ss, id, ',');
            getline(ss, stopname);
            if (id.empty() || stopname.empty()) continue;
            stop s(stoi(id), stopname);
            stops.insert({stoi(id), s});
        }
        read.close();
    }

    void LoadRouteInCSV(const string &filename) {
        ifstream read(filename);
        if (!read.is_open()) return;
        string line;
        getline(read, line); 
        while (getline(read, line)) {
            stringstream ss(line);
            string rid, source, destination, fare, distance;
            getline(ss, rid, ',');
            getline(ss, source, ',');
            getline(ss, destination, ',');
            getline(ss, fare, ',');
            getline(ss, distance);
            if (rid.empty() || source.empty() || destination.empty() || fare.empty() || distance.empty()) continue;
            stop *s = SearchStopbyName(source);
            stop *d = SearchStopbyName(destination);
            if (s && d) {
                Route r(stoi(rid), *s, *d, stoi(fare), stof(distance));
                adjlist[s->getStopID()].push_back(r);
            }
        }
        read.close();
    }

    void AddStopToCsv(const string &filename, stop s) {
        ofstream write(filename, ios::app);
        if (write.is_open()) {
            write << s.getStopID() << "," << s.getStopName() << "\n";
            write.close();
        }
    }

    void AddRouteToCsv(const string &filename, Route r) {
        ofstream write(filename, ios::app);
        if (write.is_open()) {
            write << r.getrouteid() << "," << r.getSource().getStopName() << "," << r.getDestination().getStopName() << "," << r.getFare() << "," << r.getDistance() << "\n";
            write.close();
        }
    }

    void DisplayallStops() {
        cout << "\nALL STOPS:\n";
        for (auto &r : stops) {
            cout << "ID: " << r.first << " | Name: " << r.second.getStopName() << "\n";
        }
    }

    void DisplayallRoutes() {
        cout << "\nALL ROUTES:\n";
        for (auto &r : adjlist) {
            for (auto &p : r.second) {
                cout << "From: " << stops[r.first].getStopName() << " -> To: " << p.getDestination().getStopName() << " | Dist: " << p.getDistance() << "km | Fare: " << p.getFare() << "rs\n";
            }
        }
    }

    void DisplayRoutefromStop(int StopID) {
        if (adjlist.find(StopID) == adjlist.end()) {
            cout << "No routes available from this stop.\n";
            return;
        }
        for (auto &r : adjlist[StopID]) {
            cout << "To -> " << r.getDestination().getStopName() << " | Dist: " << r.getDistance() << "km | Fare: " << r.getFare() << "rs\n";
        }
    }
};