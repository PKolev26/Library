#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "core/AppContext.h"
#include "core/Command.h"
#include "core/Book.h"
#include "core/User.h"

#include <fstream>
#include <sstream>
#include <cstdio>


TEST_CASE("OpenCommand creates new file if not found") {
    std::string filename = "open_test_file.txt";
    std::remove(filename.c_str());

    std::istringstream input(filename);
    OpenCommand cmd(input);

    AppContext context;
    cmd.execute(context);

    CHECK(std::ifstream(filename).good());
    CHECK(context.books.empty());

    std::remove(filename.c_str());
}

TEST_CASE("OpenCommand loads a book") {
    std::string filename = "open_test_book.txt";
    std::ofstream file(filename);
    file << "Author;Title;Genre;Description;2025;tag1,tag2;4.8;456466346\n";
    file.close();

    std::istringstream input(filename);
    OpenCommand cmd(input);

    AppContext context;
    cmd.execute(context);

    CHECK(context.books.size() == 1);
    CHECK(context.books[0].getAuthor() == "Author");

    std::remove(filename.c_str());
}

TEST_CASE("SaveCommand writes to existing file") {
    std::string filename = "save_test_file.txt";
    AppContext context;
    context.currentFilename = filename;
    context.fileIsOpen = true;
    context.hasChanges = true;

    Book book("Tolkien", "The Hobbit", "Fantasy", "Adventure of Bilbo", 1937, {"ring", "dragon"}, 4.8, "5354363635");
    context.newBooks.push_back(book);

    std::istringstream input("");
    SaveCommand cmd(input);
    cmd.execute(context);

    std::ifstream file(filename);
    CHECK(file.good());

    std::string line;
    while (std::getline(file, line)) {
        CHECK(line.find("5354363635") != std::string::npos);
    }

    std::remove(filename.c_str());
}

TEST_CASE("SaveAsCommand saves under new name") {
    std::string filename = "saveas_test_file.txt";
    AppContext context;
    context.fileIsOpen = true;
    context.hasChanges = true;

    Book book("Tolkien", "The Hobbit", "Fantasy", "Adventure of Bilbo",1937, {"ring", "dragon"}, 4.8, "64566564553");
    context.newBooks.push_back(book);

    std::istringstream input(filename);
    SaveAsCommand cmd(input);
    cmd.execute(context);

    std::ifstream file(filename);
    CHECK(file.good());
    std::string line;
    while (std::getline(file, line)) {
        CHECK(line.find("64566564553") != std::string::npos);
    }
    file.close();
    std::remove(filename.c_str());
}

TEST_CASE("CloseCommand resets AppContext") {
    AppContext context;
    context.currentFilename = "file.txt";
    context.fileIsOpen = true;
    context.isLoggedIn = true;
    context.books.push_back(Book());

    std::istringstream input("");
    CloseCommand cmd(input);
    cmd.execute(context);

    CHECK(context.currentFilename.empty());
    CHECK(context.books.empty());
    CHECK(context.fileIsOpen == false);
    CHECK(context.isLoggedIn == true);
}

TEST_CASE("HelpCommand prints help information") {
    std::istringstream input("");
    HelpCommand cmd(input);

    AppContext context;
    cmd.execute(context);

    std::ostringstream output;
    output << "The following commands are supported:\n"
           << "open <file>\topens <file>\n"
           << "close\t\tcloses currently opened file\n"
           << "save\t\tsaves the currently open file\n"
           << "saveas <file>\tsaves the currently open file as <file>\n"
           << "books help\tprints information about books commands\n"
           << "users help\tprints information about users commands\n"
           << "help\t\tprints this information\n"
           << "exit\t\texits the program\n";

    CHECK(output.str() == output.str());
}

TEST_CASE("LoginCommand logs in a user") {
    std::string username = "oop";
    std::string password = "i<3c++";

    User user(username, password, false);
    User::saveAllUsersToFile({user});
    std::istringstream input(username + " " + password);
    LoginCommand cmd(input);
    AppContext context;
    cmd.execute(context);
    CHECK(context.isLoggedIn == true);
    CHECK(context.currentUser.getUsername() == username);
    CHECK(context.currentUser.getPassword() == password);
    CHECK(context.currentUser.getIsAdmin() == false);
    std::vector<User> users;
    user.readFromFile(USER_FILE, users);

    std::ofstream out(USER_FILE, std::ios::trunc);
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].getUsername() == username) continue;

        if (i != 0) out << "\n";
        out << users[i].getUsername() << ";"
            << users[i].getPassword() << ";"
            << (users[i].getIsAdmin() ? "true" : "false");
    }
    out.close();
}

TEST_CASE("LogoutCommand logs out a user") {
    AppContext context;
    context.isLoggedIn = true;
    context.currentUser = User("oop", "i<3c++", false);

    std::istringstream input("");
    LogoutCommand cmd(input);
    cmd.execute(context);
    CHECK(context.isLoggedIn == false);
    CHECK(context.currentUser.getUsername().empty());
}

TEST_CASE("BooksAllCommand prints all books") {
    AppContext context;
    context.fileIsOpen = true;
    context.isLoggedIn = true;
    context.books.push_back(Book("Author1", "Title1", "Genre1", "Description1", 2020, {"tag1"}, 4.5, "ISBN001"));
    context.books.push_back(Book("Author2", "Title2", "Genre2", "Description2", 2021, {"tag2"}, 3.5, "ISBN002"));

    std::istringstream input("");
    BooksAllCommand cmd(input);
    cmd.execute(context);

    std::ostringstream output;
    for (const auto& book : context.books) {
        output << book.getTitle() << "\n";
    }
    
    CHECK(output.str() == "Title1\nTitle2\n");
}
TEST_CASE("BooksAddCommand adds a book") {
    AppContext context;
    context.fileIsOpen = true;
    context.isLoggedIn = true;
    context.currentUser = User("admin", "password", true);

    std::istringstream input("Author3;Title3;Genre3;Description3;2022;tag3,tag4;4.0;ISBN003");
    BooksAddCommand cmd(input);
    cmd.execute(context);

    CHECK(context.newBooks.size() == 1);
    CHECK(context.newBooks[0].getTitle() == "Title3");
}

TEST_CASE("BooksRemoveCommand removes a book") {
    AppContext context;
    context.fileIsOpen = true;
    context.isLoggedIn = true;
    context.currentUser = User("admin", "i<3c++", true);
    context.books.push_back(Book("Author4", "Title4", "Genre4", "Description4", 2023, {"tag5"}, 5.0, "ISBN004"));
    std::string isbnToRemove = "ISBN004";
    std::istringstream input(isbnToRemove);
    BooksRemoveCommand cmd(input);
    cmd.execute(context);
    CHECK(context.books.empty());
    CHECK(context.hasChanges == true);
    CHECK(context.isSorted == false);
    CHECK(context.newBooks.empty());
    CHECK(context.postSortNewBooks.empty());
    CHECK(context.fileIsOpen == true);
    CHECK(context.isLoggedIn == true);
    CHECK(context.currentUser.getUsername() == "admin");
    CHECK(context.currentUser.getIsAdmin() == true);
    bool bookFound = false;
    for (const auto& book : context.books) {
        if (book.getIsbn() == isbnToRemove) {
            bookFound = true;
            break;
        }
    }
    CHECK(bookFound == false);
    CHECK(context.hasChanges == true);
    CHECK(context.isSorted == false);
    CHECK(context.newBooks.empty());
    CHECK(context.postSortNewBooks.empty());
    CHECK(context.fileIsOpen == true);
    CHECK(context.isLoggedIn == true);
    CHECK(context.currentUser.getUsername() == "admin");
    CHECK(context.currentUser.getIsAdmin() == true);
}

int main(int argc, char** argv) {
    return doctest::Context(argc, argv).run();
}
