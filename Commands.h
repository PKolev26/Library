#include "Book.h"
#include "User.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class Commands {

private:
    std::string currentFilename;
    std::vector<Book> books;
    bool fileIsOpen = false;

public:

    // Open command
    void open(const std::string& filename) {

        if (fileIsOpen) {
            std::cout << "A file is already open. Please close it first." << std::endl;
            return;
        }
        
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::ofstream newFile(filename);
            if (!newFile.is_open()) {
                std::cerr << "Error creating file: " << filename << std::endl;
                std::exit(1);
            }

            newFile.close();
            books.clear();

            std::cout << "Successfully opened " << filename << std::endl;
        } else {
            books.clear();
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string author, title, genre, description, year, keywords, rating, isbn;
                if (std::getline(iss, author, ';') && std::getline(iss, title, ';') && std::getline(iss, genre, ';') &&
                    std::getline(iss, description, ';') && std::getline(iss, year, ';') &&
                    std::getline(iss, keywords, ';') && std::getline(iss, rating, ';') && std::getline(iss, isbn)) 
                {
                    Book book;
                    book.setAuthor(author);
                    book.setTitle(title);
                    book.setGenre(genre);
                    book.setDescription(description);
                    book.setYear(std::stoi(year));
                    std::vector<std::string> keywordsVector;
                    std::istringstream kss(keywords);
                    std::string keyword;
                    while (std::getline(kss, keyword, ',')) {
                        if (!keyword.empty()) {
                            keywordsVector.push_back(keyword);
                        }
                    }
                    book.setKeywords(keywordsVector);
                    book.setRating(std::stod(rating));
                    book.setIsbn(isbn);
                    
                    books.push_back(book);
                } else if (!line.empty()) {
                    std::cerr << "Error: Invalid file format in " << filename << std::endl;
                    std::exit(1);
                }
            }
            file.close();
            std::cout << "Successfully opened " << filename << std::endl;
        }
        currentFilename = filename;
        fileIsOpen = true;
    }

    // Close command
    void close() {

        if (!fileIsOpen) {
            std::cout << "No file is currently open." << std::endl;
            return;
        }

        books.clear();
        
        std::cout << "Successfully closed " << currentFilename << std::endl;
        currentFilename.clear();
        fileIsOpen = false;
    }

    // Save command
    void save() {

        if (!fileIsOpen) {
            std::cout << "No file is currently open." << std::endl;
            return;
        }

        std::ofstream file(currentFilename);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << currentFilename << std::endl;
            return;
        }

        for (const auto& book : books) {
            std::string keywordsStr;
            const auto& keywords = book.getKeywords();
            for (size_t i = 0; i < keywords.size(); ++i) {
                keywordsStr += keywords[i];
                if (i != keywords.size() - 1) keywordsStr += ",";
            }
            file << book.getAuthor() << ";"
                 << book.getTitle() << ";"
                 << book.getGenre() << ";"
                 << book.getDescription() << ";"
                 << book.getYear() << ";"
                 << keywordsStr << ";"
                 << book.getRating() << ";"
                 << book.getIsbn() << "\n";
        }

        file.close();
        std::cout << "Successfully saved " << currentFilename << std::endl;
    }

    // SaveAs command
    void saveAs(const std::string& filename) {

        if (!fileIsOpen) {
            std::ofstream newFile(filename);
            if (!newFile.is_open()) {
                std::cerr << "Error creating file: " << filename << std::endl;
                return;
            }
            newFile.close();
        }
        std::ofstream file(filename);

        for (const auto& book : books) {
            std::string keywordsStr;
            const auto& keywords = book.getKeywords();
            for (size_t i = 0; i < keywords.size(); ++i) {
                keywordsStr += keywords[i];
                if (i != keywords.size() - 1) keywordsStr += ",";
            }
            file << book.getAuthor() << ";"
                 << book.getTitle() << ";"
                 << book.getGenre() << ";"
                 << book.getDescription() << ";"
                 << book.getYear() << ";"
                 << keywordsStr << ";"
                 << book.getRating() << ";"
                 << book.getIsbn() << "\n";
        }
        file.close();
        std::cout << "Successfully saved " << filename << std::endl;
    }

    // Help command
    void help() {
        std::cout << "The following commands are supported:" << std::endl;
        std::cout << "open <file>\topens <file>" << std::endl;
        std::cout << "close\t\tcloses currently opened file" << std::endl;
        std::cout << "save\t\tsaves the currently open file" << std::endl;
        std::cout << "saveas <file>\tsaves the currently open file in <file>" << std::endl;
        std::cout << "help\t\tprints this information" << std::endl;
        std::cout << "exit\t\texists the program" << std::endl;
    }

    // Exit command
    void exit() {
        std::cout << "Exiting the program..." << std::endl;
        std::exit(0);
    }

    // Login command
    void login() {
        std::string username;
        std::string password;

        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        std::vector<User> users;
        User userObj;
        userObj.readFromFile("users.txt", users);

        for (const auto& u : users) {
            if (u.getUsername() == username && u.getPassword() == password) {
                std::cout << "Welcome, " << username << "!" << std::endl;
                return;
            }
        }
        std::cout << "Invalid username or password." << std::endl;
    }
};