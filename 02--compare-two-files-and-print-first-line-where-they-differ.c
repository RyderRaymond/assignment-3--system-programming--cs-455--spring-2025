#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 300

int main(int argc, char *argv[]) {
  FILE *first_file, *second_file;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <first_file> <second_file>\n", argv[0]);
  }

  if ((first_file = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "%s: error opening file %s:", argv[0], argv[1]);
    perror("");
    exit(1);
  }

  if ((second_file = fopen(argv[2], "r")) == NULL) {
    fprintf(stderr, "%s: error opening file %s:", argv[0], argv[1]);
    perror("");

    fclose(first_file);
    exit(2);
  }

  char line_from_first_file[MAX_LINE_LENGTH], line_from_second_file[MAX_LINE_LENGTH];

  while ( (fgets(line_from_first_file, MAX_LINE_LENGTH, first_file) != NULL) && (fgets(line_from_second_file, MAX_LINE_LENGTH, second_file) != NULL) ) {
    char* char_first_line = line_from_first_file;
    char* char_second_line = line_from_second_file;

    //Keep traversing until either the lines are
    while (*char_first_line++ == *char_second_line++) {
      if (*(char_first_line - 1) == '\n' || *(char_first_line - 1) == '\0')
        break;
    }
    char_first_line--;
    char_second_line--;

    if (*char_first_line != *char_second_line) {
      printf("%s: %s\n", argv[1], line_from_first_file);
      printf("%s: %s\n", argv[3], line_from_second_file);
      break;
    }
  }



  fclose(first_file);
  fclose(second_file);
}