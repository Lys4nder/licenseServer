#include "../include/ConfigImporter.h"
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iostream>

void Client::ConfigImporter::ReadConfig() {
    std::ifstream fileIn("../../Client/config.cfg");

    if (fileIn) {
        // Check if file is empty
        fileIn.seekg(0, std::ios::end);
        if (fileIn.tellg() == 0) {
            throw std::runtime_error("[Client]: Config file is empty!\n");
        }
        fileIn.seekg(0, std::ios::beg);

        std::stringstream ss;
        ss << fileIn.rdbuf();
        std::getline(ss, addr_);
        std::getline(ss, port_);
    } else {
        throw std::runtime_error("[Client]: Error in opening config file!\n");
    }
    std::cout << "[Client]: Config file read successfully" << std::endl;
    fileIn.close();
}