#pragma once
#include "Book.h"
#include "User.h"
#include <vector>
#include <string>

struct AppContext {
    std::string currentFilename;
    std::vector<Book> books;
    std::vector<Book> newBooks;
    bool fileIsOpen = false;
    User currentUser;
    bool isLoggedIn = false;
    bool hasChanges = false;
};
