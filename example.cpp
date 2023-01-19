#define APPLICATION_DEF
#include "engine.h"
#include "os/file.h"
#include "os/directory.h"
#include "os/graphics/buffer.h"
#include "os/editor.h"
#include "os/utilities/str_handler.h"
#include "os/terminal/terminal.h"

using namespace std;

class CoffOS : public engine::Engine {
public:
    CoffOS() {
        app_name = "coff os";
    }

private:
	//TODO:
	// * System resource manager
	//     memory
	//     CPU 
	// * UI
	//     text editor
	//     terminal
	//     buffer focus
	// * File- and program management
	// * Hardware communication
	string current_dir = "/";

	vector<Directory*> directories; // Memory management is not implemented
	vector<File*> files;            // so this will be my 'virtual' memory for dirs and files

	engine::Sprite* file_sprite = new engine::Sprite("file.png");
	engine::Sprite* dir_sprite = new engine::Sprite("folder.png");

	int const ICON_WIDTH = 16;
	int const ICON_HEIGHT = 16;
	int const TEXT_HEIGHT = 8;

	Terminal terminal{};
	Editor* editor = nullptr;

	#include "os/graphics/draw.h"

	void update_entry(Terminal& terminal) {
		if (terminal.enable_text_entry) {
            for (const auto& key : keyboard)
                if (get_key(std::get<0>(key)).pressed) {
					string c = get_key(engine::Key::SHIFT).held ? std::get<2>(key) : std::get<1>(key);
					terminal.add_char_to_entry(c);
				}
            
			if (get_key(engine::Key::LEFT).pressed)
				terminal.buffer.cursor.set_x(std::max(0, terminal.buffer.cursor.get_x() - 1));
			
			if (get_key(engine::Key::RIGHT).pressed)
				terminal.buffer.cursor.set_x(std::min(int(terminal.text_entry_string.size()), terminal.buffer.cursor.get_x() + 1));
			
			if (get_key(engine::Key::BACK).pressed && terminal.buffer.cursor.get_x() > 0) {
				terminal.text_entry_string.erase(terminal.buffer.cursor.get_x()-1, 1);
				terminal.buffer.cursor.set_x(std::max(0, terminal.buffer.cursor.get_x() - 1));
			}

			if (get_key(engine::Key::DEL).pressed && terminal.buffer.cursor.get_x() < terminal.text_entry_string.size())
				terminal.text_entry_string.erase(terminal.buffer.cursor.get_x(), 1);
        }
    }

	bool check_if_dir_exists(string dir) {
		for(Directory* d : directories) {
			//TODO: Check if directory exists
		}
		return false;
	}

	// This is a temporary function, that will be removed when stuff is stored
	// in the actual hardware memory
	void handle_message(string msg) {
		std::string action = msg.substr(0, msg.find(':'));
        std::string rest = msg.substr(msg.find(':')+1, msg.size()-1);
		if(action == "open") {
			if(check_if_dir_exists(rest)) {
				current_dir = rest;
			}
		}
	}

	void handle_response(CommandResponse cr) {
		switch(cr.get_response_type()) {
			case ResponseType::FILE_RESPONSE:
				files.push_back(cr.get_file());
				break;
			case ResponseType::DIR_RESPONSE:
				directories.push_back(cr.get_dir());
				break;
			case ResponseType::MSG_RESPONSE:
				handle_message(cr.get_msg());
				break;
			case ResponseType::ERR_RESPONSE:
				cout << cr.get_msg() << endl;
				break;
			default:
				break;
		}
	}

	/*
	void update_editor_entry(Editor* editor) {
		for (const auto& key : keyboard)
                if (get_key(std::get<0>(key)).pressed) {
                    editor->file->content[editor->active_line].insert(editor->buffer.cursor.get_x(), get_key(engine::Key::SHIFT).held ? std::get<2>(key) : std::get<1>(key));
					editor->buffer.cursor.increase_x();
				}

		if (get_key(engine::Key::LEFT).pressed)
			editor->decrease_cursor_x();
			
		if (get_key(engine::Key::RIGHT).pressed)
			editor->increase_cursor_x();

		if(get_key(engine::Key::UP).pressed)
			editor->decrease_active_line();

		if(get_key(engine::Key::DOWN).pressed)
			editor->increase_active_line();

		if (get_key(engine::Key::BACK).pressed && editor->buffer.cursor.get_x() > 0) {
			editor->file->content[editor->active_line].erase(editor->buffer.cursor.get_x()-1, 1);
			editor->buffer.cursor.set_x(std::max(0, editor->buffer.cursor.get_x() - 1));
		}

		if (get_key(engine::Key::DEL).pressed && editor->buffer.cursor.get_x() < editor->file->content[editor->active_line].size())
			editor->file->content[editor->active_line].erase(editor->buffer.cursor.get_x(), 1);
	}
	*/

public:
	bool on_create() override {
		Directory* dir = new Directory("home", "/");
		directories.push_back(dir);

		File* file = new File("README", "txt");
		files.push_back(file);

		editor = new Editor(file);

		return true;
	}

	bool on_update(float elapsed_time) override {
		fill_rect(0, 0, screen_width(), screen_height(), engine::DARK_GREY); // background
		draw_string(4, 4, current_dir, engine::BLACK);

		draw_buffer_entry(&terminal.buffer, terminal.text_entry_string);

		update_entry(terminal);
		if(get_key(engine::Key::ENTER).pressed) {
			CommandResponse response = terminal.run_cmd();
			handle_response(response);
		}

		int offset = 0;
		for(Directory* d : directories) {
			if(d->location == current_dir) {
				draw_folder(d, 4, offset+ICON_HEIGHT);
				offset+=ICON_HEIGHT;
			}
		}

		for(File* f : files) {
			draw_file(f, 4, offset+ICON_HEIGHT);
			offset+=ICON_HEIGHT;
		}

		return true;
	}
};

int main() {
    CoffOS coffos;

    if(coffos.construct(512, 512, 2, 2))
        coffos.start();

    return 0;
}