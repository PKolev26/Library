#include "CommandFactory.h"
#include "core/Command.h"
#include "core/AppContext.h"
#include <iostream>
#include <sstream>

int main() {
    
    // Initialize the application context and command factory
    AppContext context;
    CommandFactory factory;

    // Register commands with the factory
    factory.registerCommand<OpenCommand>("open");
    factory.registerCommand<SaveCommand>("save");
    factory.registerCommand<SaveAsCommand>("saveas");
    factory.registerCommand<CloseCommand>("close");
    factory.registerCommand<HelpCommand>("help");
    factory.registerCommand<BooksHelpCommand>("books help");
    factory.registerCommand<UsersHelpCommand>("users help");
    factory.registerCommand<ExitCommand>("exit");
    factory.registerCommand<LoginCommand>("login");
    factory.registerCommand<LogoutCommand>("logout");
    factory.registerCommand<BooksAllCommand>("books all");
    factory.registerCommand<BooksInfoCommand>("books info");
    factory.registerCommand<BooksFindCommand>("books find");
    factory.registerCommand<BooksSortCommand>("books sort");
    factory.registerCommand<BooksAddCommand>("books add");
    factory.registerCommand<BooksRemoveCommand>("books remove");
    factory.registerCommand<UserAddCommand>("users add");
    factory.registerCommand<UserRemoveCommand>("users remove");

    // Main command loop
    std::string line;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        std::istringstream iss(line);
        ICommand* cmd = factory.fabricate(iss);
        if (cmd) {
            cmd->execute(context);
            delete cmd;
        } else {
            std::cout << "Unknown command" << std::endl;
        }
    }

    return 0;
}
