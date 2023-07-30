#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "color.c"

void print_file(const char* filename, int show_line_numbers, int number_nonblank, int show_nonprinting, int show_tabs, int case_insensitive) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        red();
        printf("Error: ");
        resetColor();
        yellow();
        printf("Unable to open file (%s)\n", filename);
        resetColor();
        return;
    }

    char buffer[1024];
    int line_number = 1;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if ((show_line_numbers || number_nonblank) && buffer[0] != '\n') {
            printf("%6d\t", line_number);
            line_number++;
        } else {
            printf("       \t");
        }

        int i = 0;
        while (buffer[i] != '\0') {
            if (show_nonprinting && (buffer[i] < 32 || buffer[i] > 126)) {
                switch (buffer[i]) {
                    case '\n':
                        printf("$\n");
                        break;
                    case '\t':
                        if (show_tabs) {
                            printf("^I");
                        } else {
                            putchar(buffer[i]);
                        }
                        break;
                    default:
                        printf("\\x%02X", buffer[i] & 0xFF);
                        break;
                }
            } else {
                if (case_insensitive && buffer[i] >= 'A' && buffer[i] <= 'Z') {
                    putchar(buffer[i] + 32);
                } else {
                    putchar(buffer[i]);
                }
            }
            i++;
        }
    }

    fclose(file);
}

void display_version() {
    blue();
    printf("Concat command (v1.0.1)\n");
    resetColor();
}

void display_help() {
    yellow();
    printf("Usage: ");
    resetColor();
    white();
    printf("Usage: [OPTION]... [FILE]...\n");
    resetColor();
    printf("Concatenate FILE(s) and display the content on the standard output.\n\n");
    printf("  -A, --show-all            equivalent to -vET\n");
    printf("  -b, --number-nonblank     number nonempty output lines, overrides -n\n");
    printf("  -e                        display non-printing characters with $ at the end of each line\n");
    printf("  -n, --number              number all output lines\n");
    printf("  -t                        display TAB characters as ^I\n");
    printf("  -u                        (ignored)\n");
    printf("      --show-nonprinting    show non-printing characters (use escape sequences)\n");
    printf("  -v, --version             display version information and a joke\n");
    printf("      --help                display this help and exit\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {

         yellow();
         printf("Usage: ");
         resetColor();
         white();
         printf("Usage: [OPTION]... [FILE]...\n", argv[0]);
         resetColor();
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            display_help();
            return 0;
        } else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            display_version();
            blue();
            printf("\nConcat (v1.0.1) was created and developed by Cyril John Magayaga\n");
            resetColor();
            printf("\nCopyright (c) 2023 Cyril John Magayaga");
            white();
            printf("\nTagline: Why don't scientists trust atoms? Because they make up everything!\n");
            resetColor();
            return 0;
        }
    }

    int show_line_numbers = 0;
    int number_nonblank = 0;
    int show_nonprinting = 0;
    int show_tabs = 0;
    int case_insensitive = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-A") == 0) {
            show_line_numbers = 1;
        } else if (strcmp(argv[i], "-b") == 0) {
            number_nonblank = 1;
        } else if (strcmp(argv[i], "-e") == 0) {
            show_nonprinting = 1;
        } else if (strcmp(argv[i], "-n") == 0) {
            show_line_numbers = 1;
            number_nonblank = 0;
        } else if (strcmp(argv[i], "-t") == 0) {
            show_tabs = 1;
        } else if (strcmp(argv[i], "-u") == 0) {
        } else if (strcmp(argv[i], "--show-nonprinting") == 0) {
            show_nonprinting = 1;
        } else if (strcmp(argv[i], ">") == 0) {
            if (i + 1 >= argc) {
                red();
                printf("Error: ");
                resetColor();
                yellow();
                printf("Missing filename after '>'.\n");
                resetColor();
                return 1;
            }
            freopen(argv[i + 1], "w", stdout);
            i++;
        } else if (strcmp(argv[i], ">>") == 0) {
            if (i + 1 >= argc) {
                red();
                printf("Error: ");
                resetColor();
                yellow();
                printf("Missing filename after '>>'.\n");
                resetColor();
                return 1;
            }
            freopen(argv[i + 1], "a", stdout);
            i++;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-A") != 0 && strcmp(argv[i], "-b") != 0 && strcmp(argv[i], "-e") != 0 &&
            strcmp(argv[i], "-n") != 0 && strcmp(argv[i], "-t") != 0 && strcmp(argv[i], "-u") != 0 &&
            strcmp(argv[i], "--show-nonprinting") != 0 && strcmp(argv[i], "--version") != 0 && strcmp(argv[i], "--help") != 0 &&
            strcmp(argv[i], ">") != 0 && strcmp(argv[i], ">>") != 0) {
            print_file(argv[i], show_line_numbers, number_nonblank, show_nonprinting, show_tabs, case_insensitive);
        }
    }

    return 0;
}
