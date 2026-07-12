#include "Graph.h"
#include "TicketSystem.h"
#include <iostream>
using namespace std;

void InitializeCSV(const string &filename, const string &header) {
    ifstream read(filename);
    bool fileExists = read.good();
    read.close();

    if (!fileExists) {
        ofstream write(filename);
        if (write.is_open()) {
            write << header << "\n";
            write.close();
        }
    }
}

int adminisvalid(int id, string pass) {
    ifstream read("AdminInfo.csv");
    if (!read.is_open()) {
        return (id == 1234 && pass == "admin");
    }
    string line;
    getline(read, line); 
    while (getline(read, line)) {
        stringstream ss(line);
        string id1, pass1;
        getline(ss, id1, ',');
        getline(ss, pass1);
        if (!id1.empty() && stoi(id1) == id && pass == pass1) {
            read.close();
            return 1;
        }
    }
    read.close();
    return 0;
}

int main() {
    InitializeCSV("Stops.csv", "Stop-ID,Stop-Name");
    InitializeCSV("Routes.csv", "RouteID,Source-Stop,Destination-Stop,Fare,Distance");
    InitializeCSV("Tickets.csv", "Ticket-ID,Passenger-Name,Source-Stop,Destination-Stop,Fare,Distance,Itinerary-Breakdown");
    InitializeCSV("AdminInfo.csv", "AdminID,Password");
    

    graph g1;
    TicketSystemm t1(&g1);

    g1.LoadStopInCSV("Stops.csv");
    g1.LoadRouteInCSV("Routes.csv");
    t1.LoadTicketInCsv("Tickets.csv");

    int choice1 = 0;
    while (choice1 != 3) {
        cout << "\n===========================================\n";
        cout << "        BUS TRANSIT MANAGEMENT SYSTEM        \n";
        cout << "===========================================\n";
        cout << "1. Admin Portal\n2. Passenger Portal\n3. Exit Application\nEnter choice: ";
        cin >> choice1;

        if (choice1 == 1) {
            int admin_id; string pass;
            cout << "Enter Admin ID: ";  cin >> admin_id;
            cout << "Enter Password: ";  cin.ignore(); getline(cin, pass);

            if (adminisvalid(admin_id, pass)) {
                int choice2 = 0;
                while (choice2 != 5) {
                    cout << "\n--- ADMIN DASHBOARD ---\n";
                    cout << "1. View Existing Stops\n2. View Existing Routes\n3. Add New Stop\n4. Add New Route\n5. Logout\nEnter Choice: ";
                    cin >> choice2;
                    
                    if (choice2 == 1) {
                        g1.DisplayallStops();
                    } 
                    else if (choice2 == 2) {
                        g1.DisplayallRoutes();
                    } 
                    else if (choice2 == 3) {
                        int id; string name;
                        cout << "Enter New Stop ID: ";   cin >> id;
                        
                        if (g1.SearchStopbyID(id) != nullptr) {
                            cout << "Error: A stop with ID " << id << " already exists!\n";
                            continue;
                        }
                        
                        cout << "Enter Stop Name: "; cin.ignore(); getline(cin, name);
                        g1.AddStop(stop(id, name));
                        g1.AddStopToCsv("Stops.csv", stop(id, name));
                        cout << "Stop added successfully.\n";
                    } 
                    else if (choice2 == 4) {
                        int rid, sid, did, tfare; float dist;
                        cout << "Enter New Route ID: "; cin >> rid;
                        cout << "Source Stop ID: "; cin >> sid;
                        cout << "Destination Stop ID: "; cin >> did;
                        cout << "Distance (km): "; cin >> dist;
                        cout << "Fare (Rs): "; cin >> tfare;

                        stop *s = g1.SearchStopbyID(sid);
                        stop *d = g1.SearchStopbyID(did);
                        if (s && d) {
                            g1.AddRoute(Route(rid, *s, *d, tfare, dist));
                            g1.AddRouteToCsv("Routes.csv", Route(rid, *s, *d, tfare, dist));
                            cout << "Route linked and recorded successfully.\n";
                        } else {
                            cout << "Error: One or both Stop IDs do not exist. Create stops first!\n";
                        }
                    }
                }
            } else {
                cout << "Access Denied. Incorrect credentials.\n";
            }
        } else if (choice1 == 2) {
            int choice2 = 0;
            while (choice2 != 7) {
                cout << "\n--- PASSENGER NAVIGATION PANEL ---\n";
                cout << "1. Display All Stops\n2. Display All Routes\n3. Find Shortest Route (Dijkstra)\n4. Book Bus Ticket\n5. Show Ticket Details\n6. Cancel Active Ticket\n7. Back\nEnter Choice: ";
                cin >> choice2;

                if (choice2 == 1) g1.DisplayallStops();
                else if (choice2 == 2) g1.DisplayallRoutes();
                else if (choice2 == 3) {
                    int s, d;
                    cout << "Source Stop ID: "; cin >> s;
                    cout << "Destination Stop ID: "; cin >> d;
                    g1.PrintShortestPath(s, d);
                } else if (choice2 == 4) {
                    Ticket t = t1.BookTicket();
                    if (t.ticketid != 0) t1.AddTicketInCsv("Tickets.csv", t);
                } else if (choice2 == 5) {
                    int id; cout << "Enter Ticket ID: "; cin >> id;
                    t1.Showticket(id);
                } else if (choice2 == 6) {
                    int id; cout << "Enter Ticket ID to cancel: "; cin >> id;
                    t1.cancelticket(id);
                }
            }
        }
    }
    return 0;
}