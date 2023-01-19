#include "command.h"

class OpenCommand : public Command {
private:
    std::string dir;
public:
    OpenCommand(TargetType tt, std::string n, std::string d) : Command(tt, n) {
        dir = d;
    }
    CommandResponse execute() override {
        switch(target_type) {
            case TargetType::T_FILE:
                std::cout << "successfully opened file: " + name << std::endl;
                break;
            case TargetType::T_DIRECTORY: {
                std::cout << "successfully opened dir: " + name << std::endl;
                CommandResponse res{"open:"+dir+name, false};
                return res;
            }
        }
        CommandResponse err_res{"error", true};
        return err_res;
    }
};