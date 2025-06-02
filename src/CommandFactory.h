#pragma once
#include "interfaces/ICommand.h"
#include <map>
#include <functional>
#include <string>
#include <sstream>

// CommandFactory is responsible for creating command objects based on input strings.
class CommandFactory {
    std::map<std::string, std::function<ICommand*(std::istream&)>> registry;
public:
    template <typename T>
    void registerCommand(const std::string& name) {
        registry[name] = [](std::istream& is) -> ICommand* {
            return new T(is);
        };
    }

    ICommand* fabricate(std::istream& is) {
        std::string line;
        std::getline(is, line);
        std::istringstream fullLine(line);

        std::string argument, built;
        std::streampos startOfArgs;
        while (fullLine >> argument) {
            if (!built.empty()) built += " ";
            built += argument;

            auto command = registry.find(built);
            if (command != registry.end()) {
                startOfArgs = fullLine.tellg();
                std::string rest = (startOfArgs != -1 ? line.substr(static_cast<size_t>(startOfArgs)) : "");
                std::istringstream restStream(rest);
                return command->second(restStream);
            }
        }
        return nullptr;
    }
};