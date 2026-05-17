#ifndef VGA_H
#define VGA_H

void print(const char* str);
void backspace();
void clear();
void square(int x, int y, int width, int height, unsigned char color);
void frame(int x, int y, int width, int height, unsigned char color);
void window(int x, int y, int width, int height, const char* title, const char* content, unsigned char title_color, unsigned char border_color, unsigned char content_color);

#endif
