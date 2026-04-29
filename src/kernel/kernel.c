#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../../include/standart.h"

int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(unsigned char*)a - *(unsigned char*)b;
}

void swiss() {
    clear();
    print("Swiss editor (ESC to exit)\n\n");

    while (1) {
        char c = get_key();

        if (c == 27) { // ESC
            break;
        } else if (c == '\b') {
            backspace();
        } else if (c == '\n') {
            print("\n");
        } else {
            char str[2] = {c, 0};
            print(str);
        }
    }
}

void kernel_main() {
    clear();

    print("Welcome to ");
    print(DISTRO);
    print("!\nWrite help for cmd list! ");
    print("\n\n> ");

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

            // parse command
            char command[64];
            int i = 0;

            while (buffer[i] != ' ' && buffer[i] != '\0') {
                command[i] = buffer[i];
                i++;
            }
            command[i] = '\0';

            char *args = buffer + i;
            if (*args == ' ') args++;

            // commands
            if (strcmp(command, "info") == 0) {

                print("Distro: \"");
                print(DISTRO);
                print("\"\n");

                print("Version: \"");
                print(VERSION);
                print("\"\n");

                print("Authors: \"");
                print(AUTHORS);
                print("\"\n");

            } else if (strcmp(command, "echo") == 0) {

                print(args);
                print("\n");
	    } else if (strcmp(command, "swiss") == 0) {
		swiss();

            } else if (strcmp(command, "off") == 0) {

                print("You can safely turn off your PC now.\n");

                asm volatile("cli");
                while (1) asm volatile("hlt");
	    } else if (strcmp(command, "help") == 0) {
		
		print("off - turn off cpu to safety power off\n");
		print("echo - prints arguments\n");
		print("info - prints information about ts\n");
		print("clr or cls - clears all console\n");
		print("swiss - open text editor\n");

	    } else if (strcmp(command, "clr") == 0 || strcmp(command, "cls") == 0) {
		clear();

            } else {
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
