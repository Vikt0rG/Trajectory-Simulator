#include "simulation.hpp"

int main() {
    // ---------------------------------------------------------------------------------------------
    // Initilize socket for interconnection
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    } else {
        cout << "Socket initialized..." << endl;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    } else {
        cout << "Binding complete..." << endl;
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    } else {
        cout << "Awaiting connection requests..." << endl;
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    } else {
        cout << "Connection established!" << endl;
    }

    // ---------------------------------------------------------------------------------------------
    Particle electron(1, 9.109e-31, -1.602e-19, 5, 5, 1, 1, 0, 0);
    Particle uniton(2, 1.0, 1.0, 5.0, 5.0, 0.5, 0.1, 0, 0);
    // Enclosure enclosure("Steel", 5.0, 1.0, 0.0, 0.0);

    // Magnetic field parameters
    // double B0 = 1.0;
    // vector<pair<double, double>> magnet_positions = {{10, 10}, {-10, 10}, {-10, -10}, {-10, -10}};

    double dt = 1e-2;  // Time step for integration

    for (int i = 0; i < 1e4; ++i) {
        uniton.setCummulativeForceX(0.0);
        uniton.setCummulativeForceY(0.0);

        // electron.applyMagnetForce(magnet_positions, B0);
        uniton.applyRadialPotential(10, 1, 1);
        uniton.updateAcceleration();

        uniton.updatePosition(dt);
        uniton.updateVelocity(dt);
        // Output the particle's position for visualization
        cout << "-------------------------------------" << endl;
        cout << "Time step: " << i << endl;
        cout << "Position: " << uniton.getPosition().at(0) << " " << uniton.getPosition().at(1) << endl;
        cout << "Velocity: " << uniton.getVelocity().at(0) << " " << uniton.getVelocity().at(1) << endl;
        cout << "Acceleration: " << uniton.getAcceleration().at(0) << " " << uniton.getAcceleration().at(1) << endl;
        sendData(new_socket, uniton);
        this_thread::sleep_for(chrono::milliseconds(10)); // Simulate time step
    }

    close(server_fd);
    return 0;
}