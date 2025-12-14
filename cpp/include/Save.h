#pragma once
#include <fstream>
#include <nlohmann/json.hpp>

#include "Simulation.h"

using json = nlohmann::json;

namespace sim {
    static void saveSimulation(Simulation* simulation, std::string file_path) {

    }
    static void loadSimulation(Simulation* simulation, std::string file_path) {
        std::ifstream input(file_path);
        json data = json::parse(input);
    }
}