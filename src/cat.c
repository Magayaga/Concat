#include <stdio.h>
#include <stdlib.h>

void print_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", filename);
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: cat [OPTION]... [FILE]...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        print_file(argv[i]);
    }

    return 0;
}
