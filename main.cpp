#include "Book.h"
#include "User.h"
#include "Commands.h"

int main() {

    Commands commands;

    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "exit") {
            commands.exit();
        } else if (command == "help") {
            commands.help();
        } else if (command.size() >= 5 && command.substr(0, 5) == "open ") {
            std::string filename = command.substr(5);
            commands.open(filename);
        } else if (command == "close") {
            commands.close();
        } else if (command == "save") {
            commands.save();
        } else if (command.size() >= 7 && command.substr(0, 7) == "saveas ") {
            std::string filename = command.substr(7);
            commands.saveAs(filename);
        } else if (command == "login") {
            commands.login();
        } else {
            std::cout << "Unknown command: " << command << std::endl;
        }
    }

    return 0;
}