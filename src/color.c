#include <stdio.h>
#include "color.h"

// Red text
void red() {
  printf("\033[1;31m");
}

// Green text
void green() {
  printf("\033[1;32m");
}

// Yellow text
void yellow() {
  printf("\033[1;33m");
}

// Blue text
void blue() {
  printf("\033[1;34m");
}

// Purple text
void purple() {
  printf("\033[1;35m");
}

// White text
void white() {
  printf("\033[1;37m");
}

void resetColor() {
  printf("\033[0m");
}
