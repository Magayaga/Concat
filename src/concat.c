#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "color.h"

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
        int is_blank_line = buffer[0] == '\n';
        
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
                        printf("$\n");
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
                if (case_insensitive && buffer[i] >= 'A' && buffer[i] <= 'Z') {
                    putchar(buffer[i] + 32);
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
    printf("Concat command (v1.1.1)\n");
    resetColor();
}

void display_help() {
    yellow();
    printf("Usage: ");
    resetColor();
    printf("concat [OPTION]... [FILE]...\n");
    printf("Concatenate FILE(s) and display the content on the standard output.\n\n");
    printf("  -A, --show-all                         equivalent to -e -t -n\n");
    printf("  -b, --number-nonblank                  number nonempty output lines, overrides -n\n");
    printf("  -e                                     display non-printing characters with $ at the end of each line\n");
    printf("  -n, --number                           number all output lines\n");
    printf("  -t                                     display TAB characters as ^I\n");
    printf("  -u                                     (ignored)\n");
    printf("      --show-nonprinting                 show non-printing characters (use escape sequences)\n");
    printf("  -v, --version                          display version information and a joke\n");
    printf("      --help                             display this help and exit\n");
    printf("      --nl                               number lines like the nl command with additional options:\n");
    printf("           -b, --body-numbering          specify numbering for body (default: t, a, n)\n");
    printf("           -h, --header-numbering        specify numbering for header (default: a, n)\n");
    printf("           -f, --footer-numbering        specify numbering for footer (default: a, n)\n");
    printf("           -i, --join-blank-lines        specify the number of blank lines to be joined\n");
    printf("           -l, --line-number-interval    specify the interval between line numbers\n");
    printf("           -s, --number-separator        specify the separator to use between line numbers and lines\n");
    printf("           -w, --line-number-width       specify the width of the line numbers\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        yellow();
        printf("Usage: ");
        resetColor();
        printf("concat [OPTION]... [FILE]...\n");
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
	
	else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            blue();
            printf("\nConcat (v1.1.1) was created and developed by Cyril John Magayaga\n");
            resetColor();
            printf("\nCopyright (c) 2023-2024 Cyril John Magayaga");
            printf("\nTagline: Command-based utility that reads files sequentially, writing them to standard output!\n");
            return 0;
        }
	
	else if (strcmp(argv[i], "-A") == 0) {
            show_line_numbers = 1;
            show_nonprinting = 1;
            show_tabs = 1;
        }
	
	else if (strcmp(argv[i], "-b") == 0) {
            number_nonblank = 1;
        }
	
	else if (strcmp(argv[i], "-e") == 0) {
            show_nonprinting = 1;
        }
	
	else if (strcmp(argv[i], "-n") == 0) {
            show_line_numbers = 1;
            number_nonblank = 0;
        }
	
	else if (strcmp(argv[i], "-t") == 0) {
            show_tabs = 1;
        }
	
	else if (strcmp(argv[i], "-u") == 0) {
            // Ignored
        }
	
	else if (strcmp(argv[i], "--show-nonprinting") == 0) {
            show_nonprinting = 1;
        }
	
	else if (strcmp(argv[i], "-i") == 0) {
            case_insensitive = 1;
        }
	
	else if (strcmp(argv[i], "--nl") == 0) {
            nl_format = 1;
        }
	
	else if (strcmp(argv[i], "--body-numbering") == 0 || strcmp(argv[i], "-b") == 0) {
            if (++i < argc) {
                if (strcmp(argv[i], "a") == 0) {
                    body_numbering = 1;
                }
		
		else if (strcmp(argv[i], "t") == 0) {
                    body_numbering = 2;
                }
		
		else {
                    body_numbering = 0;
                }
            }
        }
	
	else if (strcmp(argv[i], "--header-numbering") == 0 || strcmp(argv[i], "-h") == 0) {
            if (++i < argc) {
                if (strcmp(argv[i], "a") == 0) {
                    header_numbering = 1;
                }
		
		else if (strcmp(argv[i], "t") == 0) {
                    header_numbering = 2;
                }
		
		else {
                    header_numbering = 0;
                }
            }
        }
	
	else if (strcmp(argv[i], "--footer-numbering") == 0 || strcmp(argv[i], "-f") == 0) {
            if (++i < argc) {
                if (strcmp(argv[i], "a") == 0) {
                    footer_numbering = 1;
                }
		
		else if (strcmp(argv[i], "t") == 0) {
                    footer_numbering = 2;
                }
		
		else {
                    footer_numbering = 0;
                }
            }
        }
	
	else if (strcmp(argv[i], "--join-blank-lines") == 0 || strcmp(argv[i], "-i") == 0) {
            if (++i < argc) {
                join_blank_lines = atoi(argv[i]);
            }
        }
	
	else if (strcmp(argv[i], "--line-number-interval") == 0 || strcmp(argv[i], "-l") == 0) {
            if (++i < argc) {
                line_number_interval = atoi(argv[i]);
            }
        } else if (strcmp(argv[i], "--number-separator") == 0 || strcmp(argv[i], "-s") == 0) {
            if (++i < argc) {
                line_number_separator = argv[i];
            }
        }
	
	else if (strcmp(argv[i], "--line-number-width") == 0 || strcmp(argv[i], "-w") == 0) {
            if (++i < argc) {
                line_number_width = atoi(argv[i]);
            }
        }
	
	else if (strcmp(argv[i], ">") == 0) {
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
        }
	
	else if (strcmp(argv[i], ">>") == 0) {
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
	
	else if (argv[i][0] != '-') {
            print_file(argv[i], show_line_numbers, number_nonblank, show_nonprinting, show_tabs, case_insensitive, nl_format, body_numbering, header_numbering, footer_numbering, join_blank_lines, line_number_interval, line_number_separator, line_number_width);
        }
    }

    return 0;
}
