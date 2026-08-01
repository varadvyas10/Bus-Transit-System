#pragma once
#include <string>
#include <iostream>
using namespace std;

class stop
{
private:
    int StopID;
    string StopName;

public:
    stop() : StopID(-1), StopName("Unknown") {}
    stop(int stopID, string stopName) : StopID(stopID), StopName(stopName) {}
    int getStopID() const { return StopID; }
    const string& getStopName() const { return StopName; } //read-only reference to the internal string — no copy 

    void DisplayStop() const
    {
        cout << "StopID:" << StopID << "\nStopName:" << StopName << endl;
    }
};

class Route
{
private:
    int RouteID;
    stop Source;
    stop Destination;
    int fare;
    float distance;

public:
    Route(int RouteID, stop Source, stop Destination, int fare, float distance)
        : Source(Source), Destination(Destination)
    {
        this->RouteID = RouteID;
        this->fare = fare;
        this->distance = distance;
    }
    float getDistance() const { return distance; }
    int getFare() const { return fare; }
    stop getSource() const { return Source; }
    stop getDestination() const { return Destination; }
    int getrouteid() const { return RouteID; }
};

class Ticket
{
private:
    int ticketid;
    string PassengerName;
    int sourceid;
    int destid;
    int fare;
    float dist;
    string routeDetails;

public:
    Ticket() : ticketid(0), PassengerName(""), sourceid(0), destid(0), fare(0), dist(0.0f), routeDetails("") {}
    Ticket(int id, const string &name, int id1, int id2, int f, float d, const string &details)
        : ticketid(id), PassengerName(name), sourceid(id1), destid(id2), fare(f), dist(d), routeDetails(details) {}

    int getTicketID() const { return ticketid; }
    string getPassengerName() const { return PassengerName; }
    int getSourceID() const { return sourceid; }
    int getDestID() const { return destid; }
    int getFare() const { return fare; }
    float getDist() const { return dist; }
    string getRouteDetails() const { return routeDetails; }
};