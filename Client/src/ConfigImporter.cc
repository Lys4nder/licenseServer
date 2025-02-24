#include "../include/ConfigImporter.h"
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iostream>

<<<<<<< HEAD
void Server::ConfigImporter::ReadConfig() {
=======
void Client::ConfigImporter::ReadConfig() {
>>>>>>> source/main
    std::ifstream fileIn("../config.cfg");

    if (fileIn) {
        // Check if file is empty
        fileIn.seekg(0, std::ios::end);
        if (fileIn.tellg() == 0) {
<<<<<<< HEAD
            throw std::runtime_error("[Server]: Config file is empty!\n");
=======
            throw std::runtime_error("[Client]: Config file is empty!\n");
>>>>>>> source/main
        }
        fileIn.seekg(0, std::ios::beg);

        std::stringstream ss;
        ss << fileIn.rdbuf();
        std::getline(ss, addr_);
        std::getline(ss, port_);
<<<<<<< HEAD
        std::getline(ss, imageFolderPath_);
    } else {
        throw std::runtime_error("[Server]: Error in opening config file!\n");
    }
    std::cout << "[Server]: Config file read successfully" << std::endl;
=======
    } else {
        throw std::runtime_error("[Client]: Error in opening config file!\n");
    }
    std::cout << "[Client]: Config file read successfully" << std::endl;
>>>>>>> source/main
    fileIn.close();
}