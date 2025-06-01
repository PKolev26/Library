#pragma once

#include <string>
#include "core/AppContext.h"

// Abstract base class for commands in the application.
class ICommand {
public:
    virtual void execute(AppContext& context) = 0;
    virtual ~ICommand() = default;
};
