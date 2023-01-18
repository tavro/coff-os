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