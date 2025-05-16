#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

class User {

    private:
    std::string username;
    std::string password;
    bool isAdmin;

    public:

    User() = default;
    
    User(std::string _username, std::string _password, bool _isAdmin) : username(_username), password(_password), isAdmin(_isAdmin) {}

    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    bool getIsAdmin() const { return isAdmin; }

    void setUsername(const std::string& _username) { username = _username; }
    void setPassword(const std::string& _password) { password = _password; }
    void setIsAdmin(bool _isAdmin) { isAdmin = _isAdmin; }

    void readFromFile(const std::string& filename, std::vector<User>& users) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string username, password, isAdminStr;
            if (std::getline(iss, username, ';') &&
                std::getline(iss, password, ';') &&
                std::getline(iss, isAdminStr)) {
                bool isAdmin = (isAdminStr == "true" || isAdminStr == "1");
                users.emplace_back(username, password, isAdmin);
            }
        }
        file.close();
    }
};