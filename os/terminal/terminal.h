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

    std::string get_standing_dir() {
        return standing_dir;
    }

    void add_char_to_entry(std::string c) {
        text_entry_string.insert(buffer.cursor.get_x(), c);
        buffer.cursor.increase_x();
    }

    void print_text_entry() {
        std::cout << ">" + text_entry_string + "\n";
    }

    void flush_text_entry() {
        text_entry_string.clear();
		buffer.cursor.set_x(0);
    }

    void add_entry_to_history() {
        history.push_back(text_entry_string);
        flush_text_entry();
    }

    CommandResponse run_cmd() {
		std::istringstream ss(text_entry_string);
    	
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
			CommandResponse err_res{"error: no target type", true};
            return err_res;
		}

		std::map<std::string, Command*> cmd_map;
    	cmd_map["create"] = new CreateCommand(tt, cmd[2], standing_dir);
    	cmd_map["open"]   = new OpenCommand  (tt, cmd[2]);
    	cmd_map["close"]  = new CloseCommand (tt, cmd[2]);

		if (cmd_map.count(cmd[0]) > 0) {
			CommandResponse res = cmd_map[cmd[0]]->execute();
            print_text_entry();     // COMMENT: while developing
            add_entry_to_history();
            return res;
		}
        CommandResponse err_res{"error: invalid command", true};
        return err_res;
	}
};

#endif