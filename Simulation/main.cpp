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
    Nuclei nuclei(1, 1.0, 2.0, 3.0, 0.7, 0.5);
    Enclosure enclosure("Steel", 5.0, 1.0, 0.0, 0.0);

    for (int i = 0; i < 1e5; ++i) {
        nuclei.updatePosition(0.1);
        nuclei.handleCollision(enclosure);
        sendData(new_socket, nuclei);
        this_thread::sleep_for(chrono::milliseconds(10)); // Simulate time step
    }

    close(server_fd);
    return 0;
}