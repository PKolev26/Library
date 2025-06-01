#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

class Book {

    private:
    // properties of the Book class
    std::string author;
    std::string title;
    std::string genre;
    std::string description;
    int year;
    std::vector<std::string> keywords;
    double rating;
    std::string isbn;

    // helper function to convert a string to lowercase
    std::string toLower(const std::string& str) const {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c) { return std::tolower(c); });
        return lowerStr;
    }

    public:

    // Default constructor
    Book() = default;
    
    // Parameterized constructor
    Book(std::string _author, std::string _title, std::string _genre, std::string _description, int _year, std::vector<std::string> _keywords,  double _rating, std::string _isbn)
        : author(_author), title(_title), genre(_genre), description(_description), year(_year), keywords(_keywords), rating(_rating), isbn(_isbn) {}

    // Getters
    std::string getAuthor() const { return author; }
    std::string getTitle() const { return title; }
    std::string getGenre() const { return genre; }
    std::string getDescription() const { return description; }
    int getYear() const { return year; }
    std::vector<std::string> getKeywords() const { return keywords; }
    double getRating() const { return rating; }
    std::string getIsbn() const { return isbn; }

    // Setters
    void setAuthor(const std::string& _author) { author = _author; }
    void setTitle(const std::string& _title) { title = _title; }
    void setGenre(const std::string& _genre) { genre = _genre; }
    void setDescription(const std::string& _description) { description = _description; }
    void setYear(int _year) { year = _year; }
    void setKeywords(const std::vector<std::string>& _keywords) { keywords = _keywords; }
    void setRating(double _rating) { rating = _rating; }
    void setIsbn(const std::string& _isbn) { isbn = _isbn; }

    // Print functions used for displaying book information
    void print() const {
        std::cout << author << "; " << title << "; " << genre << "; "<< description << "; " << isbn << std::endl;
    }

    // Print function to display book information by ISBN with all details
    void printByIsbn() const {
        
        std::cout << author << "; "
                << title << "; "
                << genre << "; "
                << description << "; "
                << year << "; ";
        for (size_t i = 0; i < keywords.size(); ++i) {
            std::cout << keywords[i];
            if (i != keywords.size() - 1) std::cout << ", ";
        }
        std::cout << "; " << rating << "; " << isbn << std::endl;
    }

    // Function to find books based on a specific option and string
    void booksFind(const std::string& option, const std::string& optionString) const {
        std::string optionLower = toLower(optionString);

        if (option == "title" && toLower(title).find(optionLower) != std::string::npos) {
            print();
        } else if (option == "author" && toLower(author).find(optionLower) != std::string::npos) {
            print();
        } else if (option == "tag") {
            for (const auto& keyword : keywords) {
                if (toLower(keyword).find(optionLower) != std::string::npos) {
                    print();
                    break;
                }
            }
        }
    }

    // Function to add a book to a file
    void addBookToFile(const std::string& filename) const {
        std::ifstream checkFile(filename, std::ios::ate);
        bool isEmpty = (checkFile.tellg() == 0);
        checkFile.close();

        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        if (!isEmpty) {
            file << "\n";
        }

        file << author << ";" << title << ";" << genre << ";" << description << ";" << year << ";";

        for (size_t i = 0; i < keywords.size(); ++i) {
            file << keywords[i];
            if (i != keywords.size() - 1)
                file << ",";
        }

        file << ";" << rating << ";" << isbn;

        file.close();
    }
};
