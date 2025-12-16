#pragma once
#include <fstream>
#include <nlohmann/json.hpp>

#include "Simulation.h"

using json = nlohmann::json;


namespace sim {
    /**
     * @brief Saves the simulation state to a file.
     * 
     * The simulation state is serialized to JSON format and written to the specified file.
     * 
     * @param simulation Pointer to the Simulation to save.
     * @param file_path Path to the file where the simulation state will be saved.
     */
    static void saveSimulation(Simulation* simulation, std::string file_path) {
        std::cout << "Save simulation to " << file_path << "\n";
        std::ofstream file(file_path);
        if (file.is_open()) {
            json data = simulation->as_json();
            file.close();
        } else {
            std::cerr << "Error: " << file_path << " is not valid!\n";
        }
    }
    /**
     * @brief Loads the simulation state from a file.
     * @param simulation Pointer to the Simulation to load into.
     * @param file_path Path to the file from which the simulation state will be loaded.
     */
    static void loadSimulation(Simulation* simulation, std::string file_path) {
        std::cout << "Load simulation from " << file_path << "\n";
        std::ifstream file(file_path);
        if (file.is_open()) {
            json data;
            file >> data;
            simulation->from_json(data);
            file.close();
        } else {
            std::cerr << "Error: " << file_path << " is not valid!\n";
        }
    }
}