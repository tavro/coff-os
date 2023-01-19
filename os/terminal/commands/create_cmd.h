#include "command.h"

class CreateCommand : public Command {
private:
    std::string dir;

public:
    CreateCommand(TargetType tt, std::string n, std::string d) : Command(tt, n) {
        dir = d;
    }

    CommandResponse execute() override {
        CommandResponse* res;
        switch(target_type) {
            case TargetType::T_FILE: {
                std::string file_name = name.substr(0, name.find('.'));
                std::string extension = name.substr(name.find('.')+1, name.size()-1);
                CommandResponse res{new File(file_name, extension)};
                std::cout << "successfully created file: " + name << std::endl;
                return res;
            }
            case TargetType::T_DIRECTORY: {
                CommandResponse res{new Directory(name, dir)};
                std::cout << "successfully created dir: " + name << std::endl;
                return res;
            }
        }
        CommandResponse err_res{"error", true};
        return err_res;
    }
};