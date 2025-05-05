#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 300

void compare_two_files(FILE *, FILE *, char *argv[]);
int compare_lines(char *, char *, char *argv[]);

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

 compare_two_files(first_file, second_file, argv);

  if (fclose(first_file) < 0) {
    fprintf(stderr, "%s: error closing file %s:", argv[0], argv[1]);
    perror("");
  }

  if (fclose(second_file) < 0) {
    fprintf(stderr, "%s: error closing file %s:", argv[0], argv[2]);
    perror("");
  }
}

void compare_two_files(FILE *first_file, FILE *second_file, char *argv[]) {
  char line_from_first_file[MAX_LINE_LENGTH], line_from_second_file[MAX_LINE_LENGTH];

  while (true) {
    int read_line_from_first_file = fgets(line_from_first_file, MAX_LINE_LENGTH, first_file) != NULL;
    int read_line_from_second_file = fgets(line_from_second_file, MAX_LINE_LENGTH, second_file) != NULL;

    if (!read_line_from_first_file && !read_line_from_second_file)
      break;

    if (!read_line_from_first_file && read_line_from_second_file) {
      printf("%s: <<reached end of file>>\n", argv[1]);
      printf("%s: %s\n", argv[2], line_from_second_file);

      break;
    }
    else if (read_line_from_first_file && !read_line_from_second_file) {
      printf("%s: %s\n", argv[1], line_from_first_file);
      printf("%s: <<reached enf of file>>\n", argv[2]);

      break;
    }
    if (compare_lines(line_from_first_file, line_from_second_file, argv) == 0) {
      return;
    }
  }
}

/** @brief compare_lines: compare two lines, returning 1 if same and 0 if not same
 *
 * @param first_line    first line to compare
 * @param second_line   second line to compare
 *
 * @return   1 if lines are the same and 0 if not the same
 */
int compare_lines(char *first_line, char *second_line, char *argv[]) {

  char *char_first_line = first_line;
  char *char_second_line = second_line;

  //Keep traversing until either the lines are not the same or we reach the end of either line
  while (*char_first_line == *char_second_line) {
    if (*char_first_line == '\n' || *char_first_line == '\0' || *char_second_line == '\n' || *char_second_line == '\0')
      break;

    char_first_line++;
    char_second_line++;
  }

  if (*char_first_line != *char_second_line) {
    printf("%s: %s\n", argv[1], first_line);
    printf("%s: %s\n", argv[2], second_line);

    return 0;
  }

  return 1;
}