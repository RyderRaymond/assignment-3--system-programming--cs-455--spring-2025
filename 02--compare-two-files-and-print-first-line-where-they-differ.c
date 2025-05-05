#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 300

int main(int argc, char *argv[]) {
  FILE *first_file, *second_file;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <first_file> <second_file>\n", argv[0]);
    exit(1);
  }

  if ((first_file = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "%s: error opening file %s:", argv[0], argv[1]);
    perror("");
    exit(2);
  }

  if ((second_file = fopen(argv[2], "r")) == NULL) {
    fprintf(stderr, "%s: error opening file %s:", argv[0], argv[1]);
    perror("");

    fclose(first_file);
    exit(3);
  }

  char line_from_first_file[MAX_LINE_LENGTH], line_from_second_file[MAX_LINE_LENGTH];

  while (true) {
    int read_line_from_first_file = fgets(line_from_first_file, MAX_LINE_LENGTH, first_file) != NULL;
    int read_line_from_second_file = fgets(line_from_second_file, MAX_LINE_LENGTH, second_file) != NULL;

    if (!read_line_from_first_file && !read_line_from_second_file)
      break;

    if (!read_line_from_first_file && read_line_from_second_file) {
      printf("%s: %s\n", argv[1], "");
      printf("%s: %s\n", argv[2], line_from_second_file);

      break;
    }
    else if (read_line_from_first_file && !read_line_from_second_file) {
      printf("%s: %s\n", argv[1], line_from_first_file);
      printf("%s: %s\n", argv[2], "");

      break;
    }

    char* char_first_line = line_from_first_file;
    char* char_second_line = line_from_second_file;

    //Keep traversing until either the lines are not the same or we reach end of line
    while (*char_first_line++ == *char_second_line++) {
      if (*(char_first_line - 1) == '\n' || *(char_first_line - 1) == '\0')
        break;
    }
    char_first_line--;
    char_second_line--;

    if (*char_first_line != *char_second_line) {
      printf("%s: %s\n", argv[1], line_from_first_file);
      printf("%s: %s\n", argv[2], line_from_second_file);

      break;
    }
  }

  if (fclose(first_file) < 0) {
    fprintf(stderr, "%s: error closing file %s:", argv[0], argv[1]);
    perror("");
  }

  if (fclose(second_file) < 0) {
    fprintf(stderr, "%s: error closing file %s:", argv[0], argv[2]);
    perror("");
  }
}