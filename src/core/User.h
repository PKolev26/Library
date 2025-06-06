#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


const std::string USER_FILE = "data/users.txt";

class User {

    // properties of the User class
    private:
    std::string username;
    std::string password;
    bool isAdmin;

    public:

    // Default constructor
    User() = default;
    
    // Parameterized constructor
    User(std::string _username, std::string _password, bool _isAdmin) : username(_username), password(_password), isAdmin(_isAdmin) {}

    // Getters 
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    bool getIsAdmin() const { return isAdmin; }

    // Setters
    void setUsername(const std::string& _username) { username = _username; }
    void setPassword(const std::string& _password) { password = _password; }
    void setIsAdmin(bool _isAdmin) { isAdmin = _isAdmin; }


    // Function to read users from a file
    void readFromFile(const std::string& filename, std::vector<User>& users) {
        users.clear();
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

    // Function to save all users to a file
    static void saveAllUsersToFile(const std::vector<User>& users) {
        std::ofstream file(USER_FILE, std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "Error opening file for writing: " << USER_FILE << std::endl;
            return;
        }

        for (size_t i = 0; i < users.size(); ++i) {
            const User& user = users[i];
            file << user.getUsername() << ";"
                << user.getPassword() << ";"
                << (user.getIsAdmin() ? "true" : "false");
            if (i < users.size() - 1)
                file << '\n';
        }

        file.close();
    }
};