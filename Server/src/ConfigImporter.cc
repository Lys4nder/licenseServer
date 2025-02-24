#include "../include/ConfigImporter.h"
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iostream>

void Server::ConfigImporter::ReadConfig() {
    std::ifstream fileIn("../config.cfg");

    if (fileIn) {
        // Check if file is empty
        fileIn.seekg(0, std::ios::end);
        if (fileIn.tellg() == 0) {
            throw std::runtime_error("[Server]: Config file is empty!\n");
        }
        fileIn.seekg(0, std::ios::beg);

        std::stringstream ss;
        ss << fileIn.rdbuf();
        std::getline(ss, addr_);
        std::getline(ss, port_);
        std::getline(ss, imageFolderPath_);
    } else {
        throw std::runtime_error("[Server]: Error in opening config file!\n");
    }
    std::cout << "[Server]: Config file read successfully" << std::endl;
    fileIn.close();
}