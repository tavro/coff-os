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
            buffer.cursor.increase_y();
            active_line++;
        }
    }

    void decrease_active_line() {
        if(active_line > 0) {
            buffer.cursor.decrease_y();
            active_line--;
        }
    }

    void increase_cursor_x() {
        if(buffer.cursor.get_x() < file->content[active_line].size()-1) {
            buffer.cursor.increase_x();
        }
        else {
            if(active_line < lines) {
                increase_active_line();
                buffer.cursor.set_x(0);
            }
        }
    }

    void decrease_cursor_x() {
        if(buffer.cursor.get_x() > 0) {
            buffer.cursor.decrease_x();
        }
        else {
            if(active_line > 0) {
                decrease_active_line();
                buffer.cursor.set_x(file->content[active_line].size()-1);
            }
        }
    }

    Editor(File* f) {
        file = f;
        file->content.push_back("this is first line of file");
        file->content.push_back("this is second line of file");
        file->content.push_back("this is third line of file");
        buffer.cursor.set_x(0);
        lines = file->content.size()-1;
    }
};