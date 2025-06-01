#pragma once
#include "Book.h"
#include "User.h"
#include <vector>
#include <string>

// defines the application context, which holds the current state of the application
struct AppContext {
    std::string currentFilename;
    std::vector<Book> books;
    std::vector<User> users;
    std::vector<Book> newBooks;
    std::vector<Book> postSortNewBooks;
    bool fileIsOpen = false;
    User currentUser;
    bool isLoggedIn = false;
    bool hasChanges = false;
    bool isSorted = false;
};
