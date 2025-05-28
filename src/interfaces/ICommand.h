#pragma once

#include <string>
#include "core/AppContext.h"

class ICommand {

public:
    virtual void execute(AppContext& context) = 0;
    virtual ~ICommand() = default;
};
