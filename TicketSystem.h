#pragma once
#include "Models.h"
#include "Graph.h"
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <tuple>
#include <vector>
#include <string>
using namespace std;

class TicketSystemm {
private:
    map<int, Ticket> Tickets;
    graph *g1;
    int counter = 1;

public:
    TicketSystemm(graph *graphptr) : g1(graphptr) {}

    int GetMaxTicketID() {
        int maxID = 0;
        for (auto &p : Tickets) {
            if (p.first > maxID) maxID = p.first;
        }
        return maxID;
    }

    Ticket BookTicket() {
        int sid, did;
        cout << "Enter source id:\n";   cin >> sid;
        cout << "Enter destination id:\n"; cin >> did;
        cout << "Enter your name:\n";
        string Pname;
        cin.ignore();
        getline(cin, Pname);

        stop *s = g1->SearchStopbyID(sid);
        stop *d = g1->SearchStopbyID(did);
        if (s && d) {
            tuple<vector<int>, float, int> result = g1->ShortestPath(sid, did);
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
                
                // Get the individual leg path values to find the exact fare
                tuple<vector<int>, float, int> legResult = g1->ShortestPath(legSrc, legDest);
                int legFare = get<2>(legResult);
                
                string srcName = g1->SearchStopbyID(legSrc)->getStopName();
                string destName = g1->SearchStopbyID(legDest)->getStopName();
                
                details += to_string(stepNum) + ") " + srcName + " to " + destName + " [Fare: " + to_string(legFare) + " Rs]; ";
                stepNum++;
            }

            Ticket t(counter, Pname, sid, did, totalfare, totaldist, details);
            Tickets.insert({t.ticketid, t});
            
            cout << "\n==========================================";
            cout << "\n       TICKET BOOKED SUCCESSFULLY!        ";
            cout << "\n==========================================";
            cout << "\nTICKET ID      : " << t.ticketid;
            cout << "\nPASSENGER NAME : " << t.PassengerName;
            cout << "\nTOTAL DISTANCE : " << t.dist << " km";
            cout << "\nTOTAL FARE     : " << t.fare << " Rs";
            cout << "\n\nTRAVEL ITINERARY ROUTE BREAKDOWN:\n";
            
            // Print details to screen replacing semicolon with clean newlines
            stringstream ss(t.routeDetails);
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
            write << t.ticketid << "," << t.PassengerName << "," << t.sourceid << "," << t.destid << "," << t.fare << "," << t.dist << "," << t.routeDetails << "\n";
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
            string id, pname, sid, did, fare, dist, details;
            getline(ss, id, ',');
            getline(ss, pname, ',');
            getline(ss, sid, ',');
            getline(ss, did, ',');
            getline(ss, fare, ',');
            getline(ss, dist, ',');
            getline(ss, details); // Read the remainder of the line as details string
            
            if (id.empty()) continue;
            Ticket t(stoi(id), pname, stoi(sid), stoi(did), stoi(fare), stof(dist), details);
            Tickets.insert({stoi(id), t});
        }
        counter = GetMaxTicketID() + 1;
        read.close();
    }

    void Showticket(int id) {
        if (Tickets.find(id) != Tickets.end()) {
            Ticket t = Tickets[id];
            cout << "\n==========================================";
            cout << "\n             TICKET DETAILS               ";
            cout << "\n==========================================";
            cout << "\nID             : " << t.ticketid;
            cout << "\nPassenger      : " << t.PassengerName;
            cout << "\nTotal Distance : " << t.dist << " km";
            cout << "\nTotal Fare     : " << t.fare << " Rs";
            cout << "\n\nItinerary Breakdown:\n";
            
            stringstream ss(t.routeDetails);
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
        if (Tickets.find(id) != Tickets.end()) {
            Tickets.erase(id);
            ofstream write("temp.csv");
            ifstream read("Tickets.csv");
            string line;
            getline(read, line);
            write << line << "\n"; 
            
            while (getline(read, line)) {
                stringstream ss(line);
                string id1;
                getline(ss, id1, ',');
                if (id1.empty()) continue;
                if (stoi(id1) != id) {
                    write << line << "\n";
                }
            }
            read.close();
            write.close();
            remove("Tickets.csv");
            rename("temp.csv", "Tickets.csv");
            cout << "Ticket with ID " << id << " has been canceled.\n";
        } else {
            cout << "Ticket ID not found.\n";
        }
    }
};