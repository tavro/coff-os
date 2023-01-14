#include <vector>
#include <string>

class Console {
public:
    Buffer buffer{128, 128, 256, 256, "Terminal", true};

    bool showing;
    bool enable_text_entry;
    
    std::string text_entry_string;

	int text_entry_cursor = 0;

	std::vector<std::string> history;

    int console_size_x;
    int console_size_y;

	int console_cursor_x;
    int console_cursor_y;

public: 
    void show() {
        if (showing)
			return;
        
        showing = true;
        enable_text_entry = true;
    }

	bool is_showing() {
        return showing;
    }

	void console_clear() {

    }

    Console() {
        
    }
};

// TODO

// update console

// if exit key pressed
// text entry enable = false
// show console = false

//