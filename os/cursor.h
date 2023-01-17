#ifndef CURSOR_DEF
#define CURSOR_DEF

class Cursor {
private:
    int x_pos;
    int y_pos;

public:
    Cursor(int x, int y) {
        x_pos = x;
        y_pos = y;
    }

    Cursor() {
        x_pos = 0;
        y_pos = 0;
    }

    int get_x() {
        return x_pos;
    }

    int get_y() {
        return y_pos;
    }

    void increase_x() {
        x_pos++;
    }

    void increase_y() {
        y_pos++;
    }

    void decrease_x() {
        x_pos--;
    }

    void decrease_y() {
        y_pos--;
    }

    void set_x(int x) {
        x_pos = x;
    }

    void set_y(int y) {
        y_pos = y;
    }
};

#endif