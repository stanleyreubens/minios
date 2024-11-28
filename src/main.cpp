// src/main.cpp
#include "simulator.hpp"
#include <iostream>

int main() {
    try {
        auto& simulator = os_sim::Simulator::getInstance();
        simulator.initialize();
        simulator.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
