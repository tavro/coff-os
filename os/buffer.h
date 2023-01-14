#include <string>
#include "cursor.h"

class Buffer {
public:
    std::string title;
    
    Cursor cursor;

    bool has_title;
    bool has_cursor;

    int x_pos;
    int y_pos;

    int width;
    int height;

    engine::Pixel color1 = engine::pixel_float(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 0.5f);
	engine::Pixel color2 = engine::pixel_float(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 0.5f);
	engine::Pixel color3 = engine::pixel_float(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 0.5f);
	engine::Pixel color4 = engine::pixel_float(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 0.5f);

    Buffer(int x, int y, int w, int h, std::string header, bool cursor) {
        title = header;
        has_title = true;
        has_cursor = cursor;

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