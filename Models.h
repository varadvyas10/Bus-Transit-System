#pragma once
#include <string>
#include <iostream>
using namespace std;

class stop {
private:
    int StopID;
    string StopName;

public:
    stop() : StopID(-1), StopName("Unknown") {} 
    stop(int StopID, string StopName) {
        this->StopID = StopID;
        this->StopName = StopName;
    }
    int getStopID() { return StopID; }
    string getStopName() { return StopName; }
    
    void DisplayStop() {
        cout << "StopID:" << StopID << "\nStopName:" << StopName << endl;
    }
};

class Route {
private:
    int RouteID;
    stop Source;
    stop Destination;
    int fare;
    float distance;

public:
    Route(int RouteID, stop Source, stop Destination, int fare, float distance)
        : Source(Source), Destination(Destination) {
        this->RouteID = RouteID;
        this->fare = fare;
        this->distance = distance;
    }
    float getDistance() { return distance; }
    int getFare() { return fare; }
    stop getSource() { return Source; }
    stop getDestination() { return Destination; }
    int getrouteid() { return RouteID; }
};

class Ticket {
public:
    int ticketid;
    string PassengerName;
    int sourceid;
    int destid;
    int fare;
    float dist;
    string routeDetails; // New field to hold the step-by-step itinerary breakdown

    Ticket() : ticketid(0), PassengerName(""), sourceid(0), destid(0), fare(0), dist(0.0f), routeDetails("") {}
    Ticket(int id, const string& name, int id1, int id2, int f, float d, const string& details)
        : ticketid(id), PassengerName(name), sourceid(id1), destid(id2), fare(f), dist(d), routeDetails(details) {}
};