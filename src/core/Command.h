#pragma once

#include "Book.h"
#include "User.h"
#include "interfaces/ICommand.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <limits>
#include <functional>
#include <conio.h>

// OpenCommand opens a file, reads its contents, and populates the AppContext with Book objects.
class OpenCommand : public ICommand {
    std::string filename;
public:
    OpenCommand(std::istream& is) {
        is >> filename;
    }
    void execute(AppContext& context) override {

        if (context.fileIsOpen) {
            std::cout << "A file is already open. Please close it before opening a new one.\n";
            return;
        }

        std::ifstream file(filename);
        if (!file.is_open()) {
            std::ofstream newFile(filename);
            if (!newFile.is_open()) {
                std::cerr << "Error creating file: " << filename << std::endl;
                return;
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
                    std::string keyword;
                    while (std::getline(kss, keyword, ',')) {
                        if (!keyword.empty()) keywordsVec.push_back(keyword);
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

// SaveCommand saves the current state of the AppContext to the currently open file.
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

        for (const auto& book : context.newBooks) {
            book.addBookToFile(context.currentFilename);
            context.books.push_back(book);
        }

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

// SaveAsCommand saves the current state of the AppContext to a new file specified by the user.
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

        for (const auto& book : context.newBooks) {
            book.addBookToFile(filename);
            context.books.push_back(book);
        }

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

// CloseCommand closes the currently open file, prompting the user if there are unsaved changes.
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

// HelpCommand prints a list of available commands and their descriptions.
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

// BooksHelpCommand prints a list of available commands related to books and their descriptions.
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

// UsersHelpCommand prints a list of available commands related to users and their descriptions.
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

// ExitCommand exits the program, prompting the user if there are unsaved changes.
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

// LoginCommand logs in a user by checking the provided username and password against the stored users.
class LoginCommand : public ICommand {

    // Function to get password input and show * instead of actual characters
    // Stackoverflow solution for password input
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
    // End of Stackoverflow solution for password input

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

        User temp;
        temp.readFromFile(USER_FILE, context.users);

        for (const auto& user : context.users) {
            if (user.getUsername() == username && user.getPassword() == password) {
                context.currentUser = user;
                context.isLoggedIn = true;
                std::cout << "Welcome, " << username << "!\n";
                return;
            }
        }
        std::cout << "Invalid username or password.\n";
    }
};

// LogoutCommand logs out the current user, if logged in.
class LogoutCommand : public ICommand {
public:
    LogoutCommand(std::istream&) {}
    void execute(AppContext& context) override {
        if (!context.isLoggedIn) {
            std::cout << "You are not logged in.\n";
            return;
        }
        context.isLoggedIn = false;
        context.currentUser = User();
        std::cout << "Successfully logged out.\n";
    }
};

// BooksAllCommand prints all books in the current context, if a file is open and the user is logged in.
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

// BooksInfoCommand prints detailed information about a book with the given ISBN, if a file is open and the user is logged in.
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

// BooksFindCommand finds books based on a specified option and value, if a file is open and the user is logged in.
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

// BooksSortCommand sorts the books in the current context based on a specified option and order, if a file is open and the user is logged in.
class BooksSortCommand : public ICommand {
    
    std::string option;
    std::string order;

    template<typename T>
    void mergeSort(std::vector<Book>& books, std::function<T(const Book&)> key, bool descending) {
        if (books.size() <= 1) return;

        size_t mid = books.size() / 2;
        std::vector<Book> left(books.begin(), books.begin() + mid);
        std::vector<Book> right(books.begin() + mid, books.end());

        mergeSort<T>(left, key, descending);
        mergeSort<T>(right, key, descending);

        size_t i = 0, j = 0, k = 0;
        while (i < left.size() && j < right.size()) {
            if ((key(left[i]) < key(right[j])) != descending)
                books[k++] = left[i++];
            else
                books[k++] = right[j++];
        }
        while (i < left.size()) books[k++] = left[i++];
        while (j < right.size()) books[k++] = right[j++];
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
        context.hasChanges = true;

        std::cout << "Books sorted by " << option << " (" << order << ")\n";
        for (const auto& b : context.books) b.printByIsbn();

        std::cout << "Use 'save' to write changes to file.\n";
    }
};

// BooksAddCommand adds a new book to the current context, if a file is open and the user is logged in as an admin.
class BooksAddCommand : public ICommand {
public:
    BooksAddCommand(std::istream&) {}

    void execute(AppContext& context) override {
        if (!context.fileIsOpen) {
            std::cout << "No file is currently open.\n";
            return;
        }
        if (!context.isLoggedIn || !context.currentUser.getIsAdmin()) {
            std::cout << "You do not have permission to add books.\n";
            return;
        }

        std::string author, title, genre, description, keywordsString, isbn;
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
            std::cout << "Invalid input. Enter a valid year: ";
        }
        std::cin.ignore();

        std::cout << "Enter keywords (comma-separated): ";
        std::getline(std::cin, keywordsString);
        std::istringstream keywordsStream(keywordsString);
        std::string keyword;
        while (std::getline(keywordsStream, keyword, ',')) {
            if (!keyword.empty()) keywords.push_back(keyword);
        }

        std::cout << "Enter rating: ";
        while (!(std::cin >> rating)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter a valid rating: ";
        }
        std::cin.ignore();

        std::cout << "Enter ISBN: "; std::getline(std::cin, isbn);

        Book book(author, title, genre, description, year, keywords, rating, isbn);

        for (const auto& book : context.books) {
            if (book.getIsbn() == isbn) {
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

// BooksRemoveCommand removes a book with the specified ISBN from the current context, if a file is open and the user is logged in as an admin.
class BooksRemoveCommand : public ICommand {
    std::string isbn;
public:
    BooksRemoveCommand(std::istream& is) {
        is >> isbn;
    }

    void execute(AppContext& context) override {
        if (!context.fileIsOpen || !context.isLoggedIn) {
            std::cout << "You need to be logged in and have a file open to remove books.\n";
            return;
        }

        if (!context.currentUser.getIsAdmin()) {
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

// UserAddCommand adds a new user to the system, if the current user is logged in as an admin.
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

        for(auto& user : context.users)
        {
            if (user.getUsername() == username) {
                std::cout << "User with this username already exists.\n";
                return;
            }
        }
        if(username.empty() || password.empty()) {
            std::cout << "Username and password cannot be empty.\n";
            return;
        }

        User newUser(username, password, false);
        context.users.push_back(newUser);
        User::saveAllUsersToFile(context.users);

        std::cout << "User " << username << " added successfully.\n";
    }
};

// UserRemoveCommand removes a user from the system by username, if the current user is logged in as an admin.
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

        auto& users = context.users;
        for (size_t i = 0; i < users.size(); ++i) {
            if (users[i].getUsername() == username) {
                if (users[i].getUsername() == context.currentUser.getUsername()) {
                    std::cout << "You cannot remove yourself while logged in.\n";
                    return;
                }

                users[i] = users.back();
                users.pop_back();

                std::ofstream file(USER_FILE, std::ios::trunc);
                if (!file.is_open()) {
                    std::cerr << "Error opening user file for writing.\n";
                    return;
                }

                User::saveAllUsersToFile(users);

                file.close();
                std::cout << "User removed successfully.\n";
                return;
            }
        }

        std::cout << "No user found with username: " << username << "\n";
    }
};
