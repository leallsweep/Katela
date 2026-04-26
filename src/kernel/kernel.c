#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../../include/standart.h"

void kernel_main() {
    clear();

    print("Welcome to ");
    print(DISTRO);
    print("!\n\n> ");

    char buffer[64];
    int index = 0;

    while (1) {
        char c = get_key();

        if (c == '\n') {
            buffer[index] = '\0';
            print("\n");

            if (index == 0) {
                print("> ");
                continue;
            }

            int found = 0;

            if (index == 4 &&
                buffer[0]=='i' &&
                buffer[1]=='n' &&
                buffer[2]=='f' &&
                buffer[3]=='o') {

                found = 1;

                print("Distro: \"");
                print(DISTRO);
                print("\"\n");

                print("Version: \"");
                print(VERSION);
                print("\"\n");

                print("Authors: \"");
                print(AUTHORS);
                print("\"\n");
            } else if ( buffer[0]=='e' &&
                buffer[1]=='c' &&
                buffer[2]=='h' &&
                buffer[3]=='o' &&
		buffer[4]==' ' &&
		buffer[5]!='\0') {

                found = 1;

                print(buffer + 5);
		print("\n");
            }

            if (!found) {
                print("standart: command not found\n");
            }

            index = 0;
            print("\n> ");
	} else if (c == '\b') {
    	    if (index > 0) {
        	index--;
                backspace();
            }
        } else {
            if (index < 63) {
                buffer[index++] = c;

                char str[2] = {c, 0};
                print(str);
	     }
        }
    }
}
