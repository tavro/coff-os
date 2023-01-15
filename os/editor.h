class Editor {
public:
    File* file;
    Buffer buffer{64, 64, 256, 256, "File Editor", true};
    
    int lines;
    int active_line;

    int get_line_amount() {
        return lines;
    }

    void increase_active_line() {
        if(active_line < lines) {
            buffer.cursor.y_pos++;
            active_line++;
        }
    }

    void decrease_active_line() {
        if(active_line > 0) {
            buffer.cursor.y_pos--;
            active_line--;
        }
    }

    void increase_cursor_x() {
        if(buffer.cursor.x_pos < file->content[active_line].size()-1) {
            buffer.cursor.x_pos++;
        }
        else {
            if(active_line < lines) {
                increase_active_line();
                buffer.cursor.x_pos = 0;
            }
        }
    }

    void decrease_cursor_x() {
        if(buffer.cursor.x_pos > 0) {
            buffer.cursor.x_pos--;
        }
        else {
            if(active_line > 0) {
                decrease_active_line();
                buffer.cursor.x_pos = file->content[active_line].size()-1;
            }
        }
    }

    Editor(File* f) {
        file = f;
        file->content.push_back("this is first line of file");
        file->content.push_back("this is second line of file");
        file->content.push_back("this is third line of file");
        buffer.cursor.x_pos = 0;
        lines = file->content.size()-1;
    }
};