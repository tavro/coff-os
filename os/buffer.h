#include <string>

class Buffer {
public:
    std::string title;
    
    bool has_title;

    int x_pos;
    int y_pos;

    int width;
    int height;

    Buffer(int x, int y, int w, int h, std::string header) {
        title = header;
        has_title = true;

        width = w;
        height = h;

        x_pos = x;
        y_pos = y;
    }

    Buffer(int x, int y, int w, int h) {
        has_title = false;

        width = w;
        height = h;

        x_pos = x;
        y_pos = y;
    }

private:
};