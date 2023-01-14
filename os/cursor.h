class Cursor {
public:
    int x_pos;
    int y_pos;

    Cursor(int x, int y) {
        x_pos = x;
        y_pos = y;
    }

    Cursor() {
        x_pos = 0;
        y_pos = 0;
    }
};