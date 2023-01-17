#ifndef TERMINAL_DEF
#define TERMINAL_DEF

#include "../graphics/buffer.h"

#include "commands/command.h"
#include "commands/open_cmd.h"
#include "commands/close_cmd.h"
#include "commands/create_cmd.h"

#include <vector>
#include <string>

class Terminal {
public:
    Buffer buffer{128, 128, 256, 256, "Terminal", true};

	std::vector<std::string> history;

    bool enable_text_entry = true;
    std::string text_entry_string = "";

    std::string standing_dir = "/";

public:
    Terminal() {
        
    }

    void report_error(std::string code) {
		std::cout << "terminal error: " + code << std::endl;
	}

    void run_cmd(std::string str) {
		std::istringstream ss(str);
    	
		std::string word;
		std::vector<std::string> cmd;
		while (ss >> word) {
			cmd.push_back(word);
		}

		TargetType tt;
		if(cmd[1] == "file") {
			tt = TargetType::T_FILE;
		}
		else if(cmd[1] == "dir") {
			tt = TargetType::T_DIRECTORY;
		}
		else {
			report_error("no target type");
			return;
		}

		std::map<std::string, Command*> cmd_map;
    	cmd_map["create"] = new CreateCommand(tt, cmd[2], standing_dir);
    	cmd_map["open"]   = new OpenCommand  (tt, cmd[2]);
    	cmd_map["close"]  = new CloseCommand (tt, cmd[2]);

		if (cmd_map.count(cmd[0]) > 0) {
			CommandResponse success = cmd_map[cmd[0]]->execute();
		} else {
			report_error("invalid command");
		}
	}
};

#endif