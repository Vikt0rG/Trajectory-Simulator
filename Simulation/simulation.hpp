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

class Particle {
    public:
        Particle(int id, double mass, double charge, double x, double y, double vx, double vy, double ax, double ay);

        void displayInfo() const;
        double getMass() const;
        vector<double> getPosition() const;
        vector<double> getVelocity() const;
        vector<double> getAcceleration() const;
        double getCummulativeForceX() const;
        double getCummulativeForceY() const;

        void setMass(double mass);
        void setCummulativeForceX(double ForceX);
        void setCummulativeForceY(double ForceY);

        void updatePosition(double dt);
        void updateVelocity(double dt);
        void updateAcceleration();

        void applyMagnetForce(const vector<pair<double, double>>& magnet_positions, double B0);
        void displayPosition() const;
        void handleCollision(const Enclosure& enclosure);

    private:
        int id;          // Unique identifier for the particle
        double mass;     // Mass of the particle
        double charge;   // Charge of the particle
        // double energy;   // Energy of the free particle
        double x, y;     // Position of the particle
        double vx, vy;   // Velocity of the particle
        double ax, ay;   // Acceleration of the particle

        double cummulative_force_x, cummulative_force_y;
};

void saveData(const Particle& particle, const string& filename);
void sendData(int sockfd, const Particle& particle);

#endif // SIMULATION_HPP