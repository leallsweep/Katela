#include "vga.h"

#define WIDTH 80
#define HEIGHT 25

static int pos = 0;
static char* video = (char*)0xB8000;

void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void update_cursor() {
    int cursor = pos / 2;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(cursor & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((cursor >> 8) & 0xFF));
}

void scroll() {
    for (int i = 0; i < (HEIGHT - 1) * WIDTH * 2; i++) {
        video[i] = video[i + WIDTH * 2];
    }

    for (int i = (HEIGHT - 1) * WIDTH * 2; i < HEIGHT * WIDTH * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = 0x07;
    }

    pos -= WIDTH * 2;
    if (pos < 0) pos = 0;
}

void square(int x, int y, int width, int height, unsigned char color) {
    int start_pos = (y * 80 + x) * 2;
    
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int byte_pos = start_pos + (row * 80 * 2) + (col * 2);
            video[byte_pos] = ' ';
            video[byte_pos + 1] = color;
        }
    }
}

void backspace() {
    if (pos <= 0) return;

    pos -= 2;

    video[pos] = ' ';
    video[pos + 1] = 0x07;
    update_cursor();
}

void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {

        if (str[i] == '\n') {
            pos += WIDTH * 2 - (pos % (WIDTH * 2));
	} else if (str[i] == '\t') {
	    for (int j = 0; j < 5; j++) {
		video[pos++] = ' ';
		video[pos++] = 0x07;
	    }
        } else {
            video[pos++] = str[i];
            video[pos++] = 0x07;
        }

        if (pos >= WIDTH * HEIGHT * 2) {
            scroll();
        }
    }
    update_cursor();
}

void clear() {
    for (int i = 0; i < WIDTH * HEIGHT * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = 0x07;
    }
    pos = 0;
    update_cursor();
}

void frame(int x, int y, int width, int height, unsigned char color) {
    int start_pos = (y * 80 + x) * 2;
    
    for (int col = 1; col < width - 1; col++) {
        int top_pos = start_pos + col * 2;
        int bottom_pos = start_pos + ((height - 1) * 80 * 2) + col * 2;
        video[top_pos] = 0xCD;
        video[top_pos + 1] = color;
        video[bottom_pos] = 0xCD;
        video[bottom_pos + 1] = color;
    }
    
    for (int row = 1; row < height - 1; row++) {
        int left_pos = start_pos + (row * 80 * 2);
        int right_pos = start_pos + (row * 80 * 2) + ((width - 1) * 2);
        video[left_pos] = 0xBA;
        video[left_pos + 1] = color;
        video[right_pos] = 0xBA;
        video[right_pos + 1] = color;
    }
    
    int tl = start_pos;
    int tr = start_pos + ((width - 1) * 2);
    int bl = start_pos + ((height - 1) * 80 * 2);
    int br = start_pos + ((height - 1) * 80 * 2) + ((width - 1) * 2);
    
    video[tl] = 0xC9;
    video[tl + 1] = color;
    video[tr] = 0xBB;
    video[tr + 1] = color;
    video[bl] = 0xC8;
    video[bl + 1] = color;
    video[br] = 0xBC;
    video[br + 1] = color;
}

void window(int x, int y, int width, int height, const char* title, const char* content, unsigned char title_color, unsigned char border_color, unsigned char content_color) {
    frame(x, y, width, height, border_color);
    
    if (title != 0) {
        int title_len = 0;
        while (title[title_len] != 0) title_len++;
        
        int start_x = x + (width - title_len) / 2;
        int pos_y = y;
        
        int start_pos = (pos_y * 80 + start_x) * 2;
        
        for (int i = 0; title[i] != 0; i++) {
            video[start_pos + i * 2] = title[i];
            video[start_pos + i * 2 + 1] = title_color;
        }
    }
    
    for (int row = 1; row < height - 1; row++) {
        square(x + 1, y + row, width - 2, 1, 0x00);
    }
    
    if (content != 0) {
        int content_x = x + 2;
        int content_y = y + 2;
        int content_pos = (content_y * 80 + content_x) * 2;
        
        for (int i = 0; content[i] != 0; i++) {
            if (content[i] == '\n') {
                content_y++;
                content_pos = (content_y * 80 + content_x) * 2;
            } else {
                video[content_pos] = content[i];
                video[content_pos + 1] = content_color;
                content_pos += 2;
            }
        }
    }
}
