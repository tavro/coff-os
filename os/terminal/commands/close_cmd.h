#include "command.h"

class CloseCommand : public Command {
public:
    CloseCommand(TargetType tt, std::string n) : Command(tt, n) {}
    CommandResponse execute() override {
        switch(target_type) {
            case TargetType::T_FILE:
                std::cout << "successfully closed file: " + name << std::endl;
                break;
            case TargetType::T_DIRECTORY:
                std::cout << "successfully closed dir: " + name << std::endl;
                break;
        }
        CommandResponse err_res{"error", true};
        return err_res;
    }
};