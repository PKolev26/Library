#pragma once

#include "Book.h"
#include "User.h"
#include "interfaces/ICommand.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <limits>
#include <conio.h>

class OpenCommand : public ICommand {
    std::string filename;
public:
    OpenCommand(std::istream& is) {
        is >> filename;
    }
    void execute(AppContext& context) override {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::ofstream newFile(filename);
            if (!newFile.is_open()) {
                std::cerr << "Error creating file: " << filename << std::endl;
                std::exit(1);
            }
            newFile.close();
            context.books.clear();
            std::cout << "Successfully opened " << filename << std::endl;
        } else {
            context.books.clear();
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string author, title, genre, description, year, keywords, rating, isbn;
                if (std::getline(iss, author, ';') && std::getline(iss, title, ';') &&
                    std::getline(iss, genre, ';') && std::getline(iss, description, ';') &&
                    std::getline(iss, year, ';') && std::getline(iss, keywords, ';') &&
                    std::getline(iss, rating, ';') && std::getline(iss, isbn)) {

                    Book book;
                    book.setAuthor(author);
                    book.setTitle(title);
                    book.setGenre(genre);
                    book.setDescription(description);
                    book.setYear(std::stoi(year));

                    std::vector<std::string> keywordsVec;
                    std::istringstream kss(keywords);
                    std::string kw;
                    while (std::getline(kss, kw, ',')) {
                        if (!kw.empty()) keywordsVec.push_back(kw);
                    }

                    book.setKeywords(keywordsVec);
                    book.setRating(std::stod(rating));
                    book.setIsbn(isbn);
                    context.books.push_back(book);
                } else if (!line.empty()) {
                    std::cerr << "Error: Invalid file format in " << filename << std::endl;
                    std::exit(1);
                }
            }
            file.close();
            std::cout << "Successfully opened " << filename << std::endl;
        }
        context.currentFilename = filename;
        context.fileIsOpen = true;
    }
};

class SaveCommand : public ICommand {
public:
    SaveCommand(std::istream&) {}

    void execute(AppContext& context) override {
        if (!context.fileIsOpen) {
            std::cout << "No file is currently open." << std::endl;
            return;
        }

        if (!context.hasChanges) {
            std::cout << "No changes to save." << std::endl;
            return;
        }

        std::ofstream file(context.currentFilename, std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << context.currentFilename << std::endl;
            return;
        }

        for (const auto& book : context.books)
            book.addBookToFile(context.currentFilename);

        for (const auto& book : context.postSortNewBooks) {
            book.addBookToFile(context.currentFilename);
            context.books.push_back(book);
        }
        context.postSortNewBooks.clear();

        context.newBooks.clear();
        context.isSorted = false;
        context.hasChanges = false;

        std::cout << "Successfully saved to " << context.currentFilename << std::endl;
    }
};

class SaveAsCommand : public ICommand {
    std::string filename;
public:
    SaveAsCommand(std::istream& is) {
        is >> filename;
    }
    void execute(AppContext& context) override {
        if (!context.fileIsOpen) {
            std::cout << "No file is currently open." << std::endl;
            return;
        }

        std::ofstream file(filename, std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        for (const auto& book : context.books)
            book.addBookToFile(filename);

        for (const auto& book : context.postSortNewBooks) {
            book.addBookToFile(filename);
            context.books.push_back(book);
        }
        context.postSortNewBooks.clear();

        context.newBooks.clear();
        context.isSorted = false;
        context.hasChanges = false;

        std::cout << "Successfully saved to " << filename << std::endl;
    }
};


class CloseCommand : public ICommand {
public:
    CloseCommand(std::istream&) {}
    void execute(AppContext& context) override {
        if (!context.fileIsOpen) {
            std::cout << "No file is currently open." << std::endl;
            return;
        }
        if (context.hasChanges) {
            std::string choice;
            std::cout << "You have unsaved changes. Are you sure you want to close file " << "? (yes/no): ";
            std::getline(std::cin, choice);
            if (choice != "yes") {
                std::cout << "Close canceled.\n";
                return;
            }
        }
        context.books.clear();
        std::cout << "Successfully closed " << context.currentFilename << std::endl;
        context.currentFilename.clear();
        context.fileIsOpen = false;
    }
};

class HelpCommand : public ICommand {
public:
    HelpCommand(std::istream&) {}
    void execute(AppContext& context) override {
        std::cout << "The following commands are supported:\n"
                  << "open <file>\topens <file>\n"
                  << "close\t\tcloses currently opened file\n"
                  << "save\t\tsaves the currently open file\n"
                  << "saveas <file>\tsaves the currently open file as <file>\n"
                  << "books help\tprints information about books commands\n"
                  << "users help\tprints information about users commands\n"
                  << "help\t\tprints this information\n"
                  << "exit\t\texits the program\n";
    }
};

class BooksHelpCommand : public ICommand {
public:
    BooksHelpCommand(std::istream&) {}
    void execute(AppContext& context) override {
        std::cout << "Books commands:\n"
                  << "books all\tprints all books\n"
                  << "books info <isbn>\tprints information about a book with the given ISBN\n"
                  << "books find <option> <value>\tfinds books by <option> (title, author, tag) and <value>\n"
                  << "books sort <option> <order>\tsorts books by <option> (title, author, year, rating) in <order> (asc, desc)\n"
                  << "books add\tadds a new book (admin only)\n"
                  << "books remove <isbn>\tremoves a book with the given ISBN (admin only)\n";
    }
};

class UsersHelpCommand : public ICommand {
public:
    UsersHelpCommand(std::istream&) {}
    void execute(AppContext& context) override {
        std::cout << "Users commands:\n"
                  << "login\t\tlogs in a user\n"
                  << "logout\t\tlogs out the current user\n"
                  << "users add <username> <password>\tadds a new user (admin only)\n"
                  << "users remove <username>\tremoves a user with the given username (admin only)\n";
    }
};

class ExitCommand : public ICommand {
public:
    ExitCommand(std::istream&) {}
    void execute(AppContext& context) override {
        if (context.hasChanges) {
            std::string choice;
            std::cout << "You have unsaved changes. Are you sure you want to exit? (yes/no): ";
            std::getline(std::cin, choice);
            if (choice != "yes") {
                std::cout << "Exit canceled.\n";
                return;
        }
    }
    std::cout << "Exiting the program..." << std::endl;
    std::exit(0);
}

};

class LoginCommand : public ICommand {
    std::string getPassword() {
        std::string password;
        char ch;

        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b";
                }
            } else {
                password += ch;
                std::cout << '*';
            }
        }
        std::cout << std::endl;
        return password;
    }
public:
    LoginCommand(std::istream&) {}
    void execute(AppContext& context) override {
        if (context.isLoggedIn) {
            std::cout << "You are already logged in.\n";
            return;
        }
        std::string username, password;
        std::cout << "Enter username: "; 
        std::getline(std::cin, username);
        std::cout << "Enter password: ";
        password = getPassword();

        std::vector<User> users;
        User temp;
        temp.readFromFile(USER_FILE, users);

        for (const auto& u : users) {
            if (u.getUsername() == username && u.getPassword() == password) {
                context.currentUser = u;
                context.isLoggedIn = true;
                std::cout << "Welcome, " << username << "!\n";
                return;
            }
        }
        std::cout << "Invalid username or password.\n";
    }
};

class LogoutCommand : public ICommand {
public:
    LogoutCommand(std::istream&) {}
    void execute(AppContext& context) override {
        if (!context.isLoggedIn) {
            std::cout << "You are not logged in.\n";
            return;
        }
        context.isLoggedIn = false;
        std::cout << "Successfully logged out.\n";
    }
};

class BooksAllCommand : public ICommand {
public:
    BooksAllCommand(std::istream&) {}
    void execute(AppContext& context) override {
        if (!context.fileIsOpen || !context.isLoggedIn) {
            std::cout << "You need to be logged in and have a file open.\n";
            return;
        }
        for (const auto& book : context.books) {
            book.print();
        }
    }
};

class BooksInfoCommand : public ICommand {
    std::string isbn;
public:
    BooksInfoCommand(std::istream& is) {
        is >> isbn;
    }
    void execute(AppContext& context) override {
        if (!context.fileIsOpen || !context.isLoggedIn) {
            std::cout << "You need to be logged in and have a file open.\n";
            return;
        }
        for (const auto& book : context.books) {
            if (book.getIsbn() == isbn)
                book.printByIsbn();
        }
    }
};

class BooksFindCommand : public ICommand {
    std::string option, value;
public:
    BooksFindCommand(std::istream& is) {
        is >> option;
        std::getline(is >> std::ws, value);
    }
    void execute(AppContext& context) override {
        if (!context.fileIsOpen || !context.isLoggedIn) {
            std::cout << "You need to be logged in and have a file open.\n";
            return;
        }
        for (const auto& book : context.books) {
            book.booksFind(option, value);
        }
    }
};

class BooksSortCommand : public ICommand {
    
    std::string option;
    std::string order;

    template<typename T>
    void mergeSort(std::vector<Book>& vec, std::function<T(const Book&)> key, bool descending) {
        if (vec.size() <= 1) return;
        
        size_t mid = vec.size() / 2;
        std::vector<Book> left(vec.begin(), vec.begin() + mid);
        std::vector<Book> right(vec.begin() + mid, vec.end());

        mergeSort<T>(left, key, descending);
        mergeSort<T>(right, key, descending);

        size_t i = 0, j = 0, k = 0;
        while (i < left.size() && j < right.size()) {
            if ((key(left[i]) < key(right[j])) != descending)
                vec[k++] = left[i++];
            else
                vec[k++] = right[j++];
        }
        while (i < left.size()) vec[k++] = left[i++];
        while (j < right.size()) vec[k++] = right[j++];
    }

public:
    BooksSortCommand(std::istream& is) {
        is >> option >> order;
        if (order != "desc") order = "asc";
    }

    void execute(AppContext& context) override {
        if (!context.fileIsOpen || !context.isLoggedIn) {
            std::cout << "You need to be logged in and have a file open.\n";
            return;
        }

        context.books.insert(context.books.end(), context.newBooks.begin(), context.newBooks.end());
        context.newBooks.clear();

        bool descending = (order == "desc");
        if (option == "title")
            mergeSort<std::string>(context.books, [](const Book& b) { return b.getTitle(); }, descending);
        else if (option == "author")
            mergeSort<std::string>(context.books, [](const Book& b) { return b.getAuthor(); }, descending);
        else if (option == "year")
            mergeSort<int>(context.books, [](const Book& b) { return b.getYear(); }, descending);
        else if (option == "rating")
            mergeSort<double>(context.books, [](const Book& b) { return b.getRating(); }, descending);
        else {
            std::cout << "Unknown sort option: " << option << "\n";
            return;
        }

        context.isSorted = true;

        std::cout << "Books sorted by " << option << " (" << order << ")\n";
        for (const auto& b : context.books) b.printByIsbn();

        std::cout << "Use 'save' to write changes to file.\n";
    }
};

class BooksAddCommand : public ICommand {
public:
    BooksAddCommand(std::istream&) {}

    void execute(AppContext& context) override {
        if (!context.fileIsOpen || !context.isLoggedIn || !context.currentUser.getIsAdmin()) {
            std::cout << "You do not have permission to add books.\n";
            return;
        }

        std::string author, title, genre, description, keywordsStr, isbn;
        int year;
        double rating;
        std::vector<std::string> keywords;

        std::cout << "Enter author: "; std::getline(std::cin, author);
        std::cout << "Enter title: "; std::getline(std::cin, title);
        std::cout << "Enter genre: "; std::getline(std::cin, genre);
        std::cout << "Enter description: "; std::getline(std::cin, description);

        std::cout << "Enter year: ";
        while (!(std::cin >> year)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter a number for year: ";
        }
        std::cin.ignore();

        std::cout << "Enter keywords (comma-separated): "; std::getline(std::cin, keywordsStr);
        std::istringstream kwStream(keywordsStr);
        std::string kw; while (std::getline(kwStream, kw, ',')) if (!kw.empty()) keywords.push_back(kw);

        std::cout << "Enter rating: ";
        while (!(std::cin >> rating)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter a number for rating: ";
        }
        std::cin.ignore();

        std::cout << "Enter ISBN: "; std::getline(std::cin, isbn);

        Book book(author, title, genre, description, year, keywords, rating, isbn);

        for (const auto& b : context.books) {
            if (b.getIsbn() == isbn) {
                std::cout << "Book with ISBN already exists.\n";
                return;
            }
        }

        if (context.isSorted) {
            context.postSortNewBooks.push_back(book);
        } else {
            context.newBooks.push_back(book);
        }

        context.hasChanges = true;

        std::cout << "Book added to memory. Use 'save' to write changes to file.\n";
    }
};

class BooksRemoveCommand : public ICommand {
    std::string isbn;
public:
    BooksRemoveCommand(std::istream& is) {
        is >> isbn;
    }

    void execute(AppContext& context) override {
        if (!context.fileIsOpen || !context.isLoggedIn || !context.currentUser.getIsAdmin()) {
            std::cout << "You do not have permission to remove books.\n";
            return;
        }

        auto& books = context.books;
        for (size_t i = 0; i < books.size(); ++i) {
            if (books[i].getIsbn() == isbn) {
                books[i] = books.back();
                books.pop_back();
                context.hasChanges = true;
                context.isSorted = false;
                std::cout << "Book removed. Use 'save' to write changes to file.\n";
                return;
            }
        }
        std::cout << "No book found with ISBN: " << isbn << "\n";
    }
};

class UserAddCommand : public ICommand {
    std::string username, password;
public:
    UserAddCommand(std::istream& is) {
        is >> username >> password;
    }
    void execute(AppContext& context) override {
        if (!context.isLoggedIn || !context.currentUser.getIsAdmin()) {
            std::cout << "You do not have permission to add users.\n";
            return;
        }
        User newUser(username, password, false);
        newUser.addUserToFile(newUser);
        std::cout << "User " << username << " added successfully.\n";
    }
};

class UserRemoveCommand : public ICommand {
    std::string username;
public:
    UserRemoveCommand(std::istream& is) {
        is >> username;
    }
    void execute(AppContext& context) override {
        if (!context.isLoggedIn || !context.currentUser.getIsAdmin()) {
            std::cout << "You do not have permission to remove users.\n";
            return;
        }
        context.currentUser.removeUserFromFile(username);
        std::cout << "User " << username << " removed successfully.\n";
    }
};