#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <string>
#include <vector>

using namespace std;

class Enclosure {
    public:
        Enclosure(std::string material, double radius, double thickness, double centerX, double centerY);

        double getRadius() const;
        std::vector<double> getCenter() const;

    private:
        std::string material;          // Enclosure Material
        double radius;                // Enclosure Radius
        double thickness;             // Enclosure wall thickness
        double centerX, centerY;      // Coordinates of the center
};

class Nuclei {
    public:
        Nuclei(int id, double mass, double x, double y, double vx, double vy);

        void displayInfo() const;
        double getMass() const;
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

#endif // SIMULATION_HPP