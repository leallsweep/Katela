#include "../drivers/vga.h"
#include "../drivers/keyboard.h"
#include "../../include/standart.h"

#define MAX_FILES 16
#define FILE_DATA_SIZE 128

struct file {
    char name[32];
    char data[FILE_DATA_SIZE];
};

struct file files[MAX_FILES];
int file_count = 0;

void strcpy(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(unsigned char*)a - *(unsigned char*)b;
}

int find_file(const char* name) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
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
		print("clear or cls - clears all console\n");
		print("swiss - open text editor\n");
		print("create {name} - create file\n");
		print("see - list files\n");
		print("set {name} {text} - write to file\n");
		print("get {name} - read file\n");

	    } else if (strcmp(command, "clear") == 0 || strcmp(command, "cls") == 0) {
		clear();
	    } else if (strcmp(command, "create") == 0) {

    		if (args[0] == '\0') {
        	    print("standart: name required\n");
    		} else if (file_count >= MAX_FILES) {
                    print("standart: file limit reached\n");
    	        } else {
        	    strcpy(files[file_count].name, args);
        	    file_count++;
        	    print("file created\n");
		}
	    } else if (strcmp(command, "see") == 0) {

    		if (file_count == 0) {
        	    print("standart: files cannot be found\n");
    		} else {
                    for (int i = 0; i < file_count; i++) {
                        print(files[i].name);
                        print("\n");
        	    }		
    		}
	    } else if (strcmp(command, "get") == 0) {

    	        int idx = find_file(args);

    		if (idx == -1) {
        	    print("standart: file not found\n");
    		} else {
        	    print(files[idx].data);
                    print("\n");
    		}
	    } else if (strcmp(command, "set") == 0) {

		char fname[32];
    		char* data = args;

    		int i = 0;
    		while (data[i] != ' ' && data[i] != '\0') {
        	    fname[i] = data[i];
                    i++;
		}
    		fname[i] = '\0';

    		if (data[i] == ' ') {
        	    data += i + 1;
    		} else {
                    print("standart: missing value\n");
        	    continue;
    		}

    		int idx = find_file(fname);

    		if (idx == -1) {
        	    if (file_count >= MAX_FILES) {
            	    	print("standart: file limit reached\n");
            	        continue;
        	    }

        	idx = file_count++;
        	strcpy(files[idx].name, fname);
    		}

    		int j = 0;
    		while (data[j] && j < FILE_DATA_SIZE - 1) {
        	   files[idx].data[j] = data[j];
        	   j++;
    		}
    	        files[idx].data[j] = '\0';

    	        print("ok\n");

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
