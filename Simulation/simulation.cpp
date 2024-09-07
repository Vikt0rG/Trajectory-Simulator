#include "simulation.hpp"

using namespace std;

// Implementation of the Enclosure constructor
Enclosure::Enclosure(string material, double radius, double thickness, double centerX, double centerY)
    : material(material), radius(radius), thickness(thickness), centerX(centerX), centerY(centerY) {}

// Getter functions for Enclosure
double Enclosure::getRadius() const {
    return radius;
}

vector<double> Enclosure::getCenter() const {
    return vector<double> {centerX, centerY};  // Return center coordinates as a vector
}

// Implementation of the Nuclei constructor
Nuclei::Nuclei(int id, double mass, double x, double y, double vx, double vy)
    : id(id), mass(mass), x(x), y(y), vx(vx), vy(vy) {}

// Displaying functions
void Nuclei::displayInfo() const {
    cout << "Nucleus ID: " << id << ", Mass: " << mass << endl;
}

void Nuclei::displayPosition() const {
    cout << "Nucleus position: (" << x << ", " << y << ")" << endl;
}

// Getter functions for Nuclei
double Nuclei::getMass() const {
    return mass;
}
vector<double> Nuclei::getPosition() const {
    return vector<double>{x, y};
}

// Setter functions for Nuclei
void Nuclei::setMass(double mass) {
    this->mass = mass;
}

// Update nucleus position based on its velocity and time step
void Nuclei::updatePosition(double timeStep) {
    x += vx * timeStep;
    y += vy * timeStep;
}

// Handle collision with the enclosure walls
void Nuclei::handleCollision(const Enclosure& enclosure) {
    double radius = enclosure.getRadius();
    vector<double> center = enclosure.getCenter();

    // Compute distance squared from the nucleus to the center of the enclosure
    double dx = x - center[0];
    double dy = y - center[1];
    double distanceSquared = dx * dx + dy * dy;
    double radiusSquared = radius * radius;

    // Check if the nucleus is outside the enclosure
    if (distanceSquared > radiusSquared) {
        cout << distanceSquared << endl;
        // Compute normal at the point of collision
        double distance = sqrt(distanceSquared);
        double normalX = dx / distance;
        double normalY = dy / distance;

        // Reflect the velocity vector
        double dotProduct = vx * normalX + vy * normalY;
        vx -= 2 * dotProduct * normalX;
        vy -= 2 * dotProduct * normalY;

        // Move the nucleus back inside the enclosure
        cout << "Collision" << endl; 
        double overlap = distance - radius;
        x -= overlap * normalX;
        y -= overlap * normalY;
    }
}

void saveData(const Nuclei& nuclei, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        double positionX = nuclei.getPosition().at(0);
        double positionY = nuclei.getPosition().at(1);
        file << positionX << "," << positionY << "\n";
        file.close();
    } else {
        cerr << "Unable to open file for writing!" << endl;
    }
}

void sendData(int sockfd, const Nuclei& nuclei) {
    double positionX = nuclei.getPosition().at(0);
    double positionY = nuclei.getPosition().at(1);
    cout << positionX << " " << positionY << endl;
    string data = to_string(positionX) + "," + to_string(positionY) + "\n";
    send(sockfd, data.c_str(), data.length(), 0);
}