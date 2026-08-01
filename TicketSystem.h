#pragma once
#include "Models.h"
#include "Graph.h"
#include "Utils.h"
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <limits>
using namespace std;


class TicketSystemm {
private:
    map<int, Ticket> Tickets;
    graph &g1;   
    int counter = 1;

public:
    TicketSystemm(graph &graphref) : g1(graphref) {}

    int GetMaxTicketID() {
        int maxID = 0;
        for (auto &p : Tickets) {
            if (p.first > maxID) maxID = p.first;
        }
        return maxID;
    }

    Ticket BookTicket() {
        
        int sid = getValidInt("Enter source id:\n");
        int did = getValidInt("Enter destination id:\n");

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string Pname = getLine("Enter your name:\n");
        Pname = sanitizeForCsv(Pname);

        stop *s = g1.SearchStopbyID(sid);
        stop *d = g1.SearchStopbyID(did);
        if (s && d) {
            tuple<vector<int>, float, int> result = g1.ShortestPath(sid, did);
            vector<int> path = get<0>(result);
            float totaldist = get<1>(result);
            int totalfare = get<2>(result);

            if (totaldist == -1) return Ticket();

            // GENERATE STEP-BY-STEP BREAKDOWN LOGIC
            string details = "";
            int stepNum = 1;

            for (size_t i = 0; i < path.size() - 1; i++) {
                int legSrc = path[i];
                int legDest = path[i+1];

                int legFare = g1.getLastEdgeFare(legDest);

                string srcName = g1.SearchStopbyID(legSrc)->getStopName();
                string destName = g1.SearchStopbyID(legDest)->getStopName();

                details += to_string(stepNum) + ") " + srcName + " to " + destName + " [Fare: " + to_string(legFare) + " Rs]; ";
                stepNum++;
            }

            Ticket t(counter, Pname, sid, did, totalfare, totaldist, details);
            Tickets.insert({t.getTicketID(), t});

            cout << "\n==========================================";
            cout << "\n       TICKET BOOKED SUCCESSFULLY!        ";
            cout << "\n==========================================";
            cout << "\nTICKET ID      : " << t.getTicketID();
            cout << "\nPASSENGER NAME : " << t.getPassengerName();
            cout << "\nTOTAL DISTANCE : " << t.getDist() << " km";
            cout << "\nTOTAL FARE     : " << t.getFare() << " Rs";
            cout << "\n\nTRAVEL ITINERARY ROUTE BREAKDOWN:\n";

            stringstream ss(t.getRouteDetails());
            string segment;
            while (getline(ss, segment, ';')) {
                if (!segment.empty() && segment != " ") cout << segment << "\n";
            }
            cout << "==========================================\n";

            counter++;
            return t;
        }
        cout << "Invalid Stop IDs.\n";
        return Ticket();
    }

    void AddTicketInCsv(const string &filename, Ticket t) {
        ofstream write(filename, ios::app);
        if (write.is_open()) {
            write << t.getTicketID() << "," << t.getPassengerName() << "," << t.getSourceID() << "," << t.getDestID() << "," << t.getFare() << "," << t.getDist() << "," << t.getRouteDetails() << "\n";
            write.close();
        }
    }

    void LoadTicketInCsv(const string &filename) {
        ifstream read(filename);
        if (!read.is_open()) return;
        string line;
        getline(read, line);
        while (getline(read, line)) {
            stringstream ss(line);
            string id, pname, sid, did, fareStr, distStr, details;
            getline(ss, id, ',');
            getline(ss, pname, ',');
            getline(ss, sid, ',');
            getline(ss, did, ',');
            getline(ss, fareStr, ',');
            getline(ss, distStr, ',');
            getline(ss, details); // remainder of the line

            if (id.empty()) continue;
            try { // Fixes bug #2: malformed rows are skipped instead of crashing the whole program
                Ticket t(stoi(id), pname, stoi(sid), stoi(did), stoi(fareStr), stof(distStr), details);
                Tickets.insert({stoi(id), t});
            } catch (const exception &e) {
                cout << "Skipping malformed ticket row: " << line << "\n";
            }
        }
        counter = GetMaxTicketID() + 1;
        read.close();
    }

    void Showticket(int id) {
        auto it = Tickets.find(id);
        if (it != Tickets.end()) {
            Ticket &t = it->second;
            cout << "\n==========================================";
            cout << "\n             TICKET DETAILS               ";
            cout << "\n==========================================";
            cout << "\nID             : " << t.getTicketID();
            cout << "\nPassenger      : " << t.getPassengerName();
            cout << "\nTotal Distance : " << t.getDist() << " km";
            cout << "\nTotal Fare     : " << t.getFare() << " Rs";
            cout << "\n\nItinerary Breakdown:\n";

            stringstream ss(t.getRouteDetails());
            string segment;
            while (getline(ss, segment, ';')) {
                if (!segment.empty() && segment != " ") cout << segment << "\n";
            }
            cout << "==========================================\n";
        } else {
            cout << "Ticket not found!\n";
        }
    }

    void cancelticket(int id) {
        if (Tickets.find(id) == Tickets.end()) {
            cout << "Ticket ID not found.\n";
            return;
        }
        Tickets.erase(id);

        ifstream read("Tickets.csv");
        ofstream write("temp.csv");
        if (!read.is_open() || !write.is_open()) { // Fixes bug #3: was previously unchecked
            cout << "Error: could not access ticket file for cancellation.\n";
            return;
        }

        string line;
        getline(read, line);
        write << line << "\n";

        while (getline(read, line)) {
            stringstream ss(line);
            string id1;
            getline(ss, id1, ',');
            if (id1.empty()) continue;
            try {
                if (stoi(id1) != id) write << line << "\n";
            } catch (const exception &e) {
                continue; // skip malformed row instead of crashing
            }
        }
        read.close();
        write.close();
        remove("Tickets.csv");
        rename("temp.csv", "Tickets.csv");
        cout << "Ticket with ID " << id << " has been canceled.\n";
    }
};