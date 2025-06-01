#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "core/Book.h"
#include "core/User.h"

TEST_CASE("Book constructor and getters") {
    Book book("Tolkien", "The Hobbit", "Fantasy", "Adventure of Bilbo",1937, {"ring", "dragon"}, 4.8, "1112223334");

    CHECK(book.getAuthor() == "Tolkien");
    CHECK(book.getTitle() == "The Hobbit");
    CHECK(book.getGenre() == "Fantasy");
    CHECK(book.getDescription() == "Adventure of Bilbo");
    CHECK(book.getYear() == 1937);
    CHECK(book.getRating() == doctest::Approx(4.8));
    CHECK(book.getIsbn() == "1112223334");
    CHECK(book.getKeywords().size() == 2);
    CHECK(book.getKeywords()[0] == "ring");
}

TEST_CASE("User constructor and getters") {
    User user("oop", "i<3c++", false);

    CHECK(user.getUsername() == "oop");
    CHECK(user.getPassword() == "i<3c++");
    CHECK(user.getIsAdmin() == false);

    user.setUsername("i<3c++");
    user.setPassword("oop");
    user.setIsAdmin(true);

    CHECK(user.getUsername() == "i<3c++");
    CHECK(user.getPassword() == "oop");
    CHECK(user.getIsAdmin() == true);
}
