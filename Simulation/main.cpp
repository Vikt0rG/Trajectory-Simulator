#include "simulation.hpp"

int main() {
    Enclosure myEnclosure("Steel", 5.0, 1.0, 0.0, 0.0);
    Nuclei myNucleus(1, 238.02891, 0.0, 0.0, 1.0, 1.0);  // Initial position and velocity

    double timeStep = 0.1; // Simulation time step
    for (int i = 0; i < 100; ++i) {  // Run simulation for 100 steps
        myNucleus.updatePosition(timeStep);
        myNucleus.handleCollision(myEnclosure);

        // Print nucleus position
        myNucleus.displayPosition();
    }

    return 0;
}