#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "color.h"

// Program name
#define PROGRAM_NAME "concat"

// Version
#define VERSION "v1.1.1"

// Cyril John Magayaga is the original author of Concat command
#define AUTHORS "Cyril John Magayaga"

void print_file(const char* filename, int show_line_numbers, int number_nonblank, int show_nonprinting, int show_tabs, int case_insensitive, int nl_format, int body_numbering, int header_numbering, int footer_numbering, int join_blank_lines, int line_number_interval, const char* line_number_separator, int line_number_width) {
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
    int nonblank_line_number = 1;
    int blank_line_count = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        int is_blank_line = (buffer[0] == '\n' || buffer[0] == '\r');

        if (nl_format) {
            if (is_blank_line) {
                if (join_blank_lines > 0) {
                    blank_line_count++;
                    if (blank_line_count > join_blank_lines) {
                        blank_line_count = 1;
                        line_number++;
                    }
                }
                
                else {
                    line_number++;
                }
            }
            
            else {
                blank_line_count = 0;
            }

            if ((line_number % line_number_interval == 0) && !is_blank_line) {
                printf("%*d%s", line_number_width, line_number, line_number_separator);
            }
            
            else if (is_blank_line && (body_numbering == 1)) {
                printf("%*d%s", line_number_width, line_number, line_number_separator);
            }
            
            else if (is_blank_line && (body_numbering == 2)) {
                printf("%*d%s", line_number_width, line_number++, line_number_separator);
            }
            
            else if (!is_blank_line) {
                printf("%*d%s", line_number_width, line_number++, line_number_separator);
            }
            
            else {
                printf("%*s%s", line_number_width, "", line_number_separator);
            }
        }
        
        else {
            if (show_line_numbers) {
                printf("%6d\t", line_number++);
            }
            
            else if (number_nonblank && !is_blank_line) {
                printf("%6d\t", nonblank_line_number++);
            }
        }

        int i = 0;
        while (buffer[i] != '\0') {
            if (show_nonprinting && (buffer[i] < 32 || buffer[i] > 126)) {
                switch (buffer[i]) {
                    case '\n':
                        if (!show_nonprinting) putchar('$');
                        break;

                    case '\t':
                        if (show_tabs) {
                            printf("^I");
                        }
                        
                        else {
                            putchar(buffer[i]);
                        }
                        break;

                    default:
                        printf("\\x%02X", buffer[i] & 0xFF);
                        break;
                }
            }
            
            else {
                if (case_insensitive && isupper(buffer[i])) {
                    putchar(tolower(buffer[i]));
                }
                
                else {
                    putchar(buffer[i]);
                }
            }
            i++;
        }

        if (buffer[i - 1] != '\n' && !show_nonprinting) {
            putchar('\n');
        }
    }

    fclose(file);
}

void display_version() {
    blue();
    printf("%s (%s)\n", PROGRAM_NAME, VERSION);
    resetColor();
    printf("Copyright (c) 2023-2024 %s", AUTHORS);
    printf("\nTagline: Command-based utility that reads files sequentially, writing them to standard output!\n");
}

void display_help() {
    const char* help_message =
        "Concatenate FILE(s), Numbering LINE(s), and display the content on the standard output.\n\n"
        "Options:\n"
        "  -A,  --show-all                        Equivalent to -e -t -n\n"
        "  -b,  --number-nonblank                 Number nonempty output lines, overrides -n\n"
        "  -e                                     Display non-printing characters with $ at the end of each line\n"
        "  -n,  --number                          Number all output lines\n"
        "  -t                                     Display TAB characters as ^I\n"
        "  -u                                     (ignored)\n"
        "       --show-nonprinting                Show non-printing characters (use escape sequences)\n"
        "  -v,  --version                         Display version information\n"
        "  -dv, --dumpversion                     Display the version of the compiler\n"
        "       --help                            Display this help and exit\n"
        "       --help=[options]                  Display help information for specific options instead of the entire help menu\n"
        "       --nl                              Number lines like the nl command with additional options:\n"
        "           -b, --body-numbering          Specify numbering for body (default: t, a, n)\n"
        "           -h, --header-numbering        Specify numbering for header (default: a, n)\n"
        "           -f, --footer-numbering        Specify numbering for footer (default: a, n)\n"
        "           -i, --join-blank-lines        Specify the number of blank lines to be joined\n"
        "           -l, --line-number-interval    Specify the interval between line numbers\n"
        "           -s, --number-separator        Specify the separator to use between line numbers and lines\n"
        "           -w, --line-number-width       Specify the width of the line numbers\n"
        "      --repl                             enter REPL (Read-Eval-Print Loop) mode\n\n"
        "Examples:\n"
        "  concat                                 Copy standard input to standard output.\n";

    yellow();
    printf("Usage: ");
    resetColor();
    printf("%s [OPTION]... [FILE]...\n", PROGRAM_NAME);
    fputs(help_message, stdout);
    printf("For bug reporting instructions, please see: ");
    blue();
    printf("<https://github.com/magayaga/concat>");
}

void show_specific_help(const char* option) {
    if (strcmp(option, "-A") == 0 || strcmp(option, "--show-all") == 0) {
        yellow();
        fputs("  -A,  --show-all                        ", stdout);
        resetColor();
        fputs("Equivalent to -e -t -n\n", stdout);
    } 
    
    else if (strcmp(option, "-b") == 0 || strcmp(option, "--number-nonblank") == 0) {
        yellow();
        fputs("  -b,  --number-nonblank                 ", stdout);
        resetColor();
        fputs("Number nonempty output lines, overrides -n\n", stdout);
    }
    
    else if (strcmp(option, "-e") == 0) {
        yellow();
        fputs("  -e                                     ", stdout);
        resetColor();
        fputs("Display non-printing characters with $ at the end of each line\n", stdout);
    }
    
    else if (strcmp(option, "-n") == 0 || strcmp(option, "--number") == 0) {
        yellow();
        fputs("  -n,  --number                          ", stdout);
        resetColor();
        fputs("Number all output lines\n", stdout);
    }
    
    else if (strcmp(option, "-t") == 0) {
        yellow();
        fputs("  -t                                     ", stdout);
        resetColor();
        fputs("Display TAB characters as ^I\n", stdout);
    }
    
    else if (strcmp(option, "-u") == 0) {
        yellow();
        fputs("  -u                                     ", stdout);
        resetColor();
        fputs("(ignored)\n", stdout);
    }
    
    else if (strcmp(option, "--show-nonprinting") == 0) {
        yellow();
        fputs("       --show-nonprinting                ", stdout);
        resetColor();
        fputs("Show non-printing characters (use escape sequences)\n", stdout);
    }
    
    else if (strcmp(option, "-v") == 0 || strcmp(option, "--version") == 0) {
        yellow();
        fputs("  -v,  --version                         ", stdout);
        resetColor();
        fputs("Display version information\n", stdout);
    }
    
    else if (strcmp(option, "--dumpversion") == 0 || strcmp(option, "-dv") == 0) {
        yellow();
        fputs("  -dv, --dumpversion                     ", stdout);
        resetColor();
        fputs("Display the version of the compiler\n", stdout);
    }
    
    else if (strcmp(option, "--help") == 0) {
        yellow();
        fputs("      --help                             ", stdout);
        resetColor();
        fputs("Display this help and exit\n", stdout);
    }
    
    else if (strcmp(option, "--nl") == 0) {
        yellow();
        fputs("      --nl                               ",stdout);
        resetColor();
        fputs("Number lines like the nl command with additional options:\n", stdout);
        fputs("           -b, --body-numbering          Specify numbering for body (default: t, a, n)\n", stdout);
        fputs("           -h, --header-numbering        Specify numbering for header (default: a, n)\n", stdout);
        fputs("           -f, --footer-numbering        Specify numbering for footer (default: a, n)\n", stdout);
        fputs("           -i, --join-blank-lines        Specify the number of blank lines to be joined\n", stdout);
        fputs("           -l, --line-number-interval    Specify the interval between line numbers\n", stdout);
        fputs("           -s, --number-separator        Specify the separator to use between line numbers and lines\n", stdout);
        fputs("           -w, --line-number-width       Specify the width of the line numbers\n", stdout);
    }
    
    else if (strcmp(option, "--repl") == 0) {
        yellow();
        fputs("      --repl                             ", stdout);
        resetColor();
        fputs("enter REPL (Read-Eval-Print Loop) mode\n", stdout);
    }
    
    else {
        yellow();
        fputs("Error: ", stdout);
        resetColor();
        fputs("Unknown option.\n", stdout);
        display_help(); // Optionally display the full help menu
    }
}


void repl() {
    printf("Entering REPL (Read-Eval-Print Loop) mode. Type 'exit' to leave.\n");
    char command[1024];

    while (true) {
        printf("concat >> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }

        // Remove newline character at the end if present
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        if (strcmp(command, "exit") == 0) {
            break;
        }

        // Split command into arguments
        char* argv[100];
        int argc = 0;
        char* token = strtok(command, " ");
        while (token != NULL && argc < 100) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }

        // Handle the command as if it were given on the command line
        for (int i = 0; i < argc; i++) {
            if (strcmp(argv[i], "--help") == 0) {
                display_help();
            }
            
            else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
                display_version();
            }
            
            else if (argv[i][0] != '-') {
                print_file(argv[i], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, "\t", 6);
            }
            
            else {
                printf("Unknown command: %s\n", argv[i]);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        yellow();
        printf("Usage: ");
        resetColor();
        printf("%s [OPTION]... [FILE]...\n", PROGRAM_NAME);
        return 1;
    }

    int show_line_numbers = 0;
    int number_nonblank = 0;
    int show_nonprinting = 0;
    int show_tabs = 0;
    int case_insensitive = 0;
    int nl_format = 0;
    int body_numbering = 0;
    int header_numbering = 0;
    int footer_numbering = 0;
    int join_blank_lines = 0;
    int line_number_interval = 1;
    const char* line_number_separator = "\t";
    int line_number_width = 6;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            display_help();
            return 0;
        }

        else if (strncmp(argv[i], "--help=", 7) == 0) {
            char *token = strtok(argv[i] + 7, ", ");
            while (token != NULL) {
                show_specific_help(token);
                token = strtok(NULL, ", ");
            }
            return 0;
        } 
        
        else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            display_version();
            return 0;
        }
        
        else if (strcmp(argv[i], "--repl") == 0) {
            repl();
            return 0;
        }
        
        else if (strcmp(argv[i], "--show-all") == 0) {
            show_nonprinting = 1;
            show_tabs = 1;
            show_line_numbers = 1;
        }
        
        else if (strcmp(argv[i], "--number-nonblank") == 0) {
            number_nonblank = 1;
        }
        
        else if (strcmp(argv[i], "--show-nonprinting") == 0) {
            show_nonprinting = 1;
        }
        
        else if (strcmp(argv[i], "--show-tabs") == 0) {
            show_tabs = 1;
        }
        
        else if (strcmp(argv[i], "--case-insensitive") == 0) {
            case_insensitive = 1;
        }
        
        else if (strcmp(argv[i], "--nl") == 0) {
            nl_format = 1;
        }
        
        else if (strcmp(argv[i], "--body-numbering") == 0) {
            if (i + 1 < argc) {
                body_numbering = atoi(argv[++i]);
            }
            
            else {
                fprintf(stderr, "Error: --body-numbering requires an argument.\n");
                return 1;
            }
        }
        
        else if (strcmp(argv[i], "--header-numbering") == 0) {
            if (i + 1 < argc) {
                header_numbering = atoi(argv[++i]);
            }
            
            else {
                fprintf(stderr, "Error: --header-numbering requires an argument.\n");
                return 1;
            }
        }
        
        else if (strcmp(argv[i], "--footer-numbering") == 0) {
            if (i + 1 < argc) {
                footer_numbering = atoi(argv[++i]);
            }
            
            else {
                fprintf(stderr, "Error: --footer-numbering requires an argument.\n");
                return 1;
            }
        }
        
        else if (strcmp(argv[i], "--join-blank-lines") == 0) {
            if (i + 1 < argc) {
                join_blank_lines = atoi(argv[++i]);
            }
            
            else {
                fprintf(stderr, "Error: --join-blank-lines requires an argument.\n");
                return 1;
            }
        }
        
        else if (strcmp(argv[i], "--line-number-interval") == 0) {
            if (i + 1 < argc) {
                line_number_interval = atoi(argv[++i]);
            }
            
            else {
                fprintf(stderr, "Error: --line-number-interval requires an argument.\n");
                return 1;
            }
        }
        
        else if (strcmp(argv[i], "--number-separator") == 0) {
            if (i + 1 < argc) {
                line_number_separator = argv[++i];
            }
            
            else {
                fprintf(stderr, "Error: --number-separator requires an argument.\n");
                return 1;
            }
        }
        
        else if (strcmp(argv[i], "--line-number-width") == 0) {
            if (i + 1 < argc) {
                line_number_width = atoi(argv[++i]);
            }
            
            else {
                fprintf(stderr, "Error: --line-number-width requires an argument.\n");
                return 1;
            }
        }
        
        else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Unknown option %s\n", argv[i]);
            return 1;
        }
        
        else {
            // Treat remaining arguments as filenames
            print_file(argv[i], show_line_numbers, number_nonblank, show_nonprinting, show_tabs, case_insensitive, nl_format, body_numbering, header_numbering, footer_numbering, join_blank_lines, line_number_interval, line_number_separator, line_number_width);
        }
    }

    return 0;
}
