#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <thread>

using namespace std;

class Enclosure {
    public:
        Enclosure(string material, double radius, double thickness, double centerX, double centerY);

        double getRadius() const;
        vector<double> getCenter() const;

    private:
        string material;          // Enclosure Material
        double radius;                // Enclosure Radius
        double thickness;             // Enclosure wall thickness
        double centerX, centerY;      // Coordinates of the center
};

class Nuclei {
    public:
        Nuclei(int id, double mass, double x, double y, double vx, double vy);

        void displayInfo() const;
        double getMass() const;
        vector<double> getPosition() const;
        void setMass(double mass);
        void updatePosition(double timeStep);
        void displayPosition() const;
        void handleCollision(const Enclosure& enclosure);

    private:
        int id;          // Unique identifier for the nucleus
        double mass;     // Mass of the nucleus
        double x, y;     // Position of the nucleus
        double vx, vy;   // Velocity of the nucleus
};

void saveData(const Nuclei& nuclei, const string& filename);
void sendData(int sockfd, const Nuclei& nuclei);

#endif // SIMULATION_HPP