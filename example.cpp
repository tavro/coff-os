#define APPLICATION_DEF
#include "engine.h"
#include "os/file.h"
#include "os/directory.h"
#include "os/buffer.h"
#include "os/console.h"

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
    //     directories
	//     terminal
	// * File- and program management
	// * Hardware communication
	vector<Directory*> directories;
	vector<File*> files;

	Buffer* buffer = new Buffer(128, 128, 256, 256, "test buffer");

	engine::Sprite* file_sprite = new engine::Sprite("file.png");
	engine::Sprite* dir_sprite = new engine::Sprite("folder.png");

	int const ICON_WIDTH = 16;
	int const ICON_HEIGHT = 16;
	int const TEXT_HEIGHT = 8;

	Console console{};

	void draw_file(File* f, int x, int y) {
		draw_sprite(x, y, file_sprite, 1, 0);
		draw_string(x + ICON_WIDTH, y + ICON_HEIGHT - TEXT_HEIGHT, f->name + "." + f->type, engine::BLACK);
	}

	void draw_folder(Directory* dir, int x, int y) {
		draw_sprite(x, y, dir_sprite, 1, 0);
		draw_string(x + ICON_WIDTH, y + ICON_HEIGHT - TEXT_HEIGHT, dir->name, engine::BLACK);
	}

	void draw_buffer(Buffer* buffer) {
		gradient_fill_rect_decal({(float)buffer->x_pos, (float)buffer->y_pos}, {(float)buffer->width, (float)buffer->height}, engine::pixel_float(0, 0, 0.5f, 0.5f), engine::pixel_float(0, 0, 0.25f, 0.5f), engine::pixel_float(0, 0, 0.25f, 0.5f), engine::pixel_float(0, 0, 0.25f, 0.5f));
		draw_rect_decal({(float)buffer->x_pos, (float)buffer->y_pos}, {(float)buffer->width, (float)buffer->height}, engine::BLACK);

		if(buffer->has_title) {
			draw_string_decal({(float)(buffer->x_pos+TEXT_HEIGHT/2), (float)(buffer->y_pos+TEXT_HEIGHT/2)}, /*buffer->title*/console.text_entry_string, engine::WHITE, { 1.0f, 1.0f });
		}
		fill_rect_decal(engine::float_vector_2d(buffer->x_pos+TEXT_HEIGHT/2 + float((console.text_entry_cursor))*8.0f, buffer->y_pos+TEXT_HEIGHT/2), engine::float_vector_2d(8, 8), engine::BLACK);
	}

	void update_entry(Console& console) {
		if (console.showing && console.enable_text_entry) {
            for (const auto& key : keyboard)
                if (get_key(std::get<0>(key)).pressed) {
                    console.text_entry_string.insert(console.text_entry_cursor, get_key(engine::Key::SHIFT).held ? std::get<2>(key) : std::get<1>(key));
					console.text_entry_cursor++;
				}
            
            if (get_key(engine::Key::ENTER).pressed) {
                std::cout << ">" + console.text_entry_string + "\n";
				console.history.push_back(console.text_entry_string);
                console.text_entry_string.clear();
				console.text_entry_cursor = 0;
            }

			if (get_key(engine::Key::LEFT).pressed)
				console.text_entry_cursor = std::max(0, console.text_entry_cursor - 1);
			
			if (get_key(engine::Key::RIGHT).pressed)
				console.text_entry_cursor = std::min(int(console.text_entry_string.size()), console.text_entry_cursor + 1);
			
			if (get_key(engine::Key::BACK).pressed && console.text_entry_cursor > 0) {
				console.text_entry_string.erase(console.text_entry_cursor-1, 1);
				console.text_entry_cursor = std::max(0, console.text_entry_cursor - 1);
			}

			if (get_key(engine::Key::DEL).pressed && console.text_entry_cursor < console.text_entry_string.size())
				console.text_entry_string.erase(console.text_entry_cursor, 1);
        }
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
				directories.push_back(new Directory(cmd[2], "/"));
			}
			else if(cmd[1] == "file") {
				files.push_back(new File(cmd[2], cmd[3]));
			}
		}
		else if(cmd[0] == "open") {
			if(cmd[1] == "dir") {

			}
			else if(cmd[1] == "file") {
				
			}
		}
	}
public:
	bool on_create() override {
		Directory* dir = new Directory("folder", "/");
		directories.push_back(dir);

		File* file = new File("text_file", "txt");
		files.push_back(file);

		console.show();
		return true;
	}

	bool on_update(float elapsed_time) override {
		fill_rect(0, 0, screen_width(), screen_height(), engine::DARK_GREY);

		draw_buffer(buffer);
		update_entry(console);

		if(get_key(engine::Key::ENTER).pressed) {
			run_cmd(console.history[console.history.size()-1]);
		}

		int offset = 0;
		for(Directory* d : directories) {
			draw_folder(d, 4, offset+ICON_HEIGHT);
			offset+=ICON_HEIGHT;
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