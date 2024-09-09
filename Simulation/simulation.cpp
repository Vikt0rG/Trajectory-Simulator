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

// Implementation of the Particle constructor
Particle::Particle(int id, double mass, double charge, double x, double y, double vx, double vy, double ax, double ay)
    : id(id), mass(mass), charge(charge), x(x), y(y), vx(vx), vy(vy), ax(ax), ay(ay) {}

// Displaying functions
void Particle::displayInfo() const {
    cout << "Particle ID: " << id << ", Mass: " << mass << endl;
}
void Particle::displayPosition() const {
    cout << "Particle position: (" << x << ", " << y << ")" << endl;
}

// Getter functions for Particle
double Particle::getMass() const { return mass; }
vector<double> Particle::getPosition() const { return vector<double>{x, y}; }
vector<double> Particle::getVelocity() const { return vector<double>{vx, vy}; }
vector<double> Particle::getAcceleration() const { return vector<double>{ax, ay}; }
double Particle::getCummulativeForceX() const { return cummulative_force_x; }
double Particle::getCummulativeForceY() const { return cummulative_force_y; }

// Setter functions for Particle
void Particle::setMass(double value) { mass = value; }
void Particle::setCummulativeForceX(double ForceX) { cummulative_force_x = ForceX; }
void Particle::setCummulativeForceY(double ForceY) { cummulative_force_y = ForceY; }

// Update particle kinematics
void Particle::updatePosition(double dt) {
    x += vx * dt + 1/2 * ax * pow(dt, 2);
    y += vy * dt + 1/2 * ay * pow(dt, 2);
}
void Particle::updateVelocity(double dt) {
    vx += ax * dt;
    vy += ay * dt;
}
void Particle::updateAcceleration() {
    ax += cummulative_force_x / mass;
    ay += cummulative_force_y / mass;
}

void Particle::applyMagnetForce(const vector<pair<double, double>>& magnet_positions, double B0) {

    // Initialize variables
    vector<pair<double, double>> fields(magnet_positions.size());
    vector<pair<double, double>> forces(fields.size());
    double cummulative_field_x = 0;
    double cummulative_field_y = 0;

    // Iterate over each magnet and calculate its field
    for (size_t i{0}; i < fields.size(); ++i) {
        double dx = x - magnet_positions.at(i).first;
        double dy = y - magnet_positions.at(i).second;
        double r_squared = dx*dx + dy*dy;
        if (r_squared == 0) continue; // Avoid division by zero

        // Biot-Savart
        fields.at(i).first = B0 * dy / pow(r_squared, 1.5);
        fields.at(i).second = -B0 * dx / pow(r_squared, 1.5);
    }
    cout << "=====================================" << endl;
    cout << "Magnetic fields: " << endl;
    for (const auto& field : fields) {
        cout << "(" << field.first << ", " << field.second << ")" << endl;
        cummulative_field_x += field.first;
        cummulative_field_y += field.second;
    }
    cout << "Cummulative field: " << cummulative_field_x << ", " << cummulative_field_y << endl;

    // Iterate over each field and calculate its force
    for (size_t i{0}; i < forces.size(); ++i) {
        forces.at(i).first = charge * (vy * fields.at(i).second);
        forces.at(i).second = - charge * (vx * fields.at(i).first);
    }
    cout << "-------------------------------------" << endl;
    cout << "Magnetic forces: " << endl;
    for (const auto& force : forces) {
        cout << "(" << force.first << ", " << force.second << ")" << endl;
        cummulative_force_x += force.first;
        cummulative_force_y += force.second;
    }
    cout << "Cummulative force: " << cummulative_force_x << ", " << cummulative_force_y << endl;
}

// Handle collision with the enclosure walls
void Particle::handleCollision(const Enclosure& enclosure) {
    double radius = enclosure.getRadius();
    vector<double> center = enclosure.getCenter();

    // Compute distance squared from the particle to the center of the enclosure
    double dx = x - center[0];
    double dy = y - center[1];
    double distanceSquared = dx * dx + dy * dy;
    double radiusSquared = radius * radius;

    // Check if the particle is outside the enclosure
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

        // Move the particle back inside the enclosure
        cout << "Collision" << endl; 
        double overlap = distance - radius;
        x -= overlap * normalX;
        y -= overlap * normalY;
    }
}

// TODO: #1 Confinement: Add repulsive potential well

void saveData(const Particle& particle, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        double positionX = particle.getPosition().at(0);
        double positionY = particle.getPosition().at(1);
        file << positionX << "," << positionY << "\n";
        file.close();
    } else {
        cerr << "Unable to open file for writing!" << endl;
    }
}

void sendData(int sockfd, const Particle& particle) {
    double positionX = particle.getPosition().at(0);
    double positionY = particle.getPosition().at(1);
    string data = to_string(positionX) + "," + to_string(positionY) + "\n";
    send(sockfd, data.c_str(), data.length(), 0);
}