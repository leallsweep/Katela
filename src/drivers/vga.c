#include "vga.h"

#define WIDTH 80
#define HEIGHT 25

static int pos = 0;
static char* video = (char*)0xB8000;

void scroll() {
    for (int i = 0; i < (HEIGHT - 1) * WIDTH * 2; i++) {
        video[i] = video[i + WIDTH * 2];
    }

    for (int i = (HEIGHT - 1) * WIDTH * 2; i < HEIGHT * WIDTH * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = 0x07;
    }

    pos -= WIDTH * 2;
}

void backspace() {
    if (pos <= 0) return;

    pos -= 2;

    video[pos] = ' ';
    video[pos + 1] = 0x07;
}

void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {

        if (str[i] == '\n') {
            pos += WIDTH * 2 - (pos % (WIDTH * 2));
        } else {
            video[pos++] = str[i];
            video[pos++] = 0x07;
        }

        if (pos >= WIDTH * HEIGHT * 2) {
            scroll();
        }
    }
}

void clear() {
    for (int i = 0; i < WIDTH * HEIGHT * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = 0x07;
    }
    pos = 0;
}
