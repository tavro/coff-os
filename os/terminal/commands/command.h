#ifndef CMD_DEF
#define CMD_DEF

#include "target_type.h"
#include "command_response.h"

#include <string>

class Command {
protected:
    TargetType target_type;
    std::string name;

public:
    Command(TargetType tt, std::string name) : target_type{tt}, name{name} { }
    
    virtual ~Command() {}
    virtual CommandResponse execute() = 0;
};

#endif