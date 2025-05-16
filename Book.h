#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class Book {

    private:
    std::string author;
    std::string title;
    std::string genre;
    std::string description;
    int year;
    std::vector<std::string> keywords;
    double rating;
    std::string isbn;

    public:

    Book() = default;
    
    Book(std::string _author, std::string _title, std::string _genre, std::string _description, int _year, std::vector<std::string> _keywords,  double _rating, std::string _isbn)
        : author(_author), title(_title), genre(_genre), description(_description), year(_year), keywords(_keywords), rating(_rating), isbn(_isbn) {}

    std::string getAuthor() const { return author; }
    std::string getTitle() const { return title; }
    std::string getGenre() const { return genre; }
    std::string getDescription() const { return description; }
    int getYear() const { return year; }
    std::vector<std::string> getKeywords() const { return keywords; }
    double getRating() const { return rating; }
    std::string getIsbn() const { return isbn; }


    void setAuthor(const std::string& _author) { author = _author; }
    void setTitle(const std::string& _title) { title = _title; }
    void setGenre(const std::string& _genre) { genre = _genre; }
    void setDescription(const std::string& _description) { description = _description; }
    void setYear(int _year) { year = _year; }
    void setKeywords(const std::vector<std::string>& _keywords) { keywords = _keywords; }
    void setRating(double _rating) { rating = _rating; }
    void setIsbn(const std::string& _isbn) { isbn = _isbn; }
};