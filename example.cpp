#define APPLICATION_DEF
#include "engine.h"
#include "os/file.h"
#include "os/directory.h"
#include "os/graphics/buffer.h"
#include "os/console.h"
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
	//          response messages
	//     buffer focus
	// * File- and program management
	// * Hardware communication
	Buffer* focused_buffer;
	vector<Buffer*> opened_buffers;
	int buffer_index = 0;

	string current_dir = "/";

	vector<Directory*> directories;
	vector<File*> files;

	engine::Sprite* file_sprite = new engine::Sprite("file.png");
	engine::Sprite* dir_sprite = new engine::Sprite("folder.png");

	int const ICON_WIDTH = 16;
	int const ICON_HEIGHT = 16;
	int const TEXT_HEIGHT = 8;

	Terminal* terminal;
	Console console{};
	Editor* editor = nullptr;

	void draw_file(File* f, int x, int y) {
		draw_sprite(x, y, file_sprite, 1, 0);
		draw_string(x + ICON_WIDTH, y + ICON_HEIGHT - TEXT_HEIGHT, f->name + "." + f->type, engine::BLACK);
	}

	void draw_folder(Directory* dir, int x, int y) {
		draw_sprite(x, y, dir_sprite, 1, 0);
		draw_string(x + ICON_WIDTH, y + ICON_HEIGHT - TEXT_HEIGHT, dir->name, engine::BLACK);
	}

	void draw_buffer(Buffer* buffer) {
		gradient_fill_rect_decal({(float)buffer->x_pos, (float)buffer->y_pos}, {(float)buffer->width, (float)buffer->height}, buffer->color1, buffer->color2, buffer->color3, buffer->color4);
		draw_rect_decal({(float)buffer->x_pos, (float)buffer->y_pos}, {(float)buffer->width, (float)buffer->height}, engine::BLACK);

		if(buffer->has_title) {
			draw_string_decal({(float)(buffer->x_pos+TEXT_HEIGHT/2), (float)(buffer->y_pos-TEXT_HEIGHT)}, buffer->title, engine::WHITE, { 1.0f, 1.0f });
		}

		if(buffer->has_cursor) {
			fill_rect_decal(engine::float_vector_2d(buffer->x_pos+TEXT_HEIGHT/2 + (float)(buffer->cursor.get_x())*8.0f, buffer->y_pos+TEXT_HEIGHT/2 + (float)(buffer->cursor.get_y())*8.0f), engine::float_vector_2d(8, 8), engine::BLACK);
		}
	}

	void draw_buffer_entry(Buffer* buffer, string entry) {
		draw_buffer(buffer);
		draw_string_decal({(float)(buffer->x_pos+TEXT_HEIGHT/2), (float)(buffer->y_pos+TEXT_HEIGHT/2)}, entry, engine::WHITE, { 1.0f, 1.0f });
	}

	void draw_editor(Editor* editor) {
		draw_buffer(&editor->buffer);
		int index = 0;
		for(string line : editor->file->content) {
			draw_string_decal({(float)(editor->buffer.x_pos+TEXT_HEIGHT/2), (float)(editor->buffer.y_pos+TEXT_HEIGHT/2) + TEXT_HEIGHT*index}, line, engine::WHITE, { 1.0f, 1.0f });
			index++;
		}
	}

	// TODO: Refactor
	void update_entry(Console& console) {
		if (console.showing && console.enable_text_entry) {
            for (const auto& key : keyboard)
                if (get_key(std::get<0>(key)).pressed) {
                    console.text_entry_string.insert(console.buffer.cursor.get_x(), get_key(engine::Key::SHIFT).held ? std::get<2>(key) : std::get<1>(key));
					console.buffer.cursor.increase_x();
				}
            
            if (get_key(engine::Key::ENTER).pressed) {
                std::cout << ">" + console.text_entry_string + "\n";
				console.history.push_back(console.text_entry_string);
                console.text_entry_string.clear();
				console.buffer.cursor.set_x(0);
            }

			if (get_key(engine::Key::LEFT).pressed)
				console.buffer.cursor.set_x(std::max(0, console.buffer.cursor.get_x() - 1));
			
			if (get_key(engine::Key::RIGHT).pressed)
				console.buffer.cursor.set_x(std::min(int(console.text_entry_string.size()), console.buffer.cursor.get_x() + 1));
			
			if (get_key(engine::Key::BACK).pressed && console.buffer.cursor.get_x() > 0) {
				console.text_entry_string.erase(console.buffer.cursor.get_x()-1, 1);
				console.buffer.cursor.set_x(std::max(0, console.buffer.cursor.get_x() - 1));
			}

			if (get_key(engine::Key::DEL).pressed && console.buffer.cursor.get_x() < console.text_entry_string.size())
				console.text_entry_string.erase(console.buffer.cursor.get_x(), 1);
        }
    }

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

	/*
	void report_error(string reason) {
		cout << "terminal error: " + reason << endl;
	}

	void run_cmd(string str) {
		istringstream ss(str);
    	
		string word;
		vector<string> cmd;
		while (ss >> word) {
			cmd.push_back(word);
		}

		if(cmd[0] == "create") {
			if(cmd[1] == "dir") {
				directories.push_back(new Directory(cmd[2], current_dir));
			}
			else if(cmd[1] == "file") {
				files.push_back(new File(cmd[2], cmd[3]));
			}
		}
		else if(cmd[0] == "open") {
			if(cmd[1] == "dir") {
				for(Directory* dir : directories)
					if(dir->name == cmd[2])
						current_dir += cmd[2] + "/";
			}
			else if(cmd[1] == "file") {
				for(File* f : files) {
					if(f->name == cmd[2]) {
						f->is_open = true;
						focused_buffer = &editor->buffer;
						opened_buffers.push_back(&editor->buffer);
						break;
					}
				}
			}
		}
		else if(cmd[0] == "close") {
			if(cmd[1] == "dir") {
				if(current_dir != "/") {
					remove_last_substring(current_dir);
				}
			}
			else if(cmd[1] == "file") {
				for(File* f : files) {
					if(f->name == cmd[2]) {
						if(f->is_open) {
							f->is_open = false;

							for(Buffer* b : opened_buffers)
								if(b == &editor->buffer)
									// TODO: Remove buffer

							focused_buffer = &console.buffer;
							break;
						}
					}
				}
			}
		}
		else {
			report_error("invalid command");
		}
	}
	*/
public:
	bool on_create() override {
		terminal = new Terminal();

		Directory* dir = new Directory("folder", "/");
		directories.push_back(dir);

		File* file = new File("text_file", "txt");
		files.push_back(file);

		editor = new Editor(file);

		console.show();
		focused_buffer = &console.buffer;
		opened_buffers.push_back(&console.buffer);
		return true;
	}

	bool on_update(float elapsed_time) override {
		if(get_key(engine::Key::TAB).pressed) {
			if(buffer_index < opened_buffers.size()-1)
				buffer_index++;
			else {
				buffer_index = 0;
			}
			focused_buffer = opened_buffers[buffer_index];
		}

		fill_rect(0, 0, screen_width(), screen_height(), engine::DARK_GREY);
		draw_string(4, 4, current_dir, engine::BLACK);

		draw_buffer_entry(&console.buffer, console.text_entry_string);
		if(focused_buffer == &console.buffer)
			update_entry(console);

		if(get_key(engine::Key::ENTER).pressed) {
			Terminal* t = new Terminal();
			t->run_cmd(console.history[console.history.size()-1]);
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
			if(f->is_open) {
				draw_editor(editor);
				if(focused_buffer == &editor->buffer)
					update_editor_entry(editor);
			}
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