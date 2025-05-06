#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 300

void compare_two_files(FILE *, FILE *, char *argv[]);
int compare_lines(char *, char *, char *argv[], int, unsigned int);

/** @brief main: main program logic
 *
 * @param argc
 * @param argv
 * @return 0 for successful exit, 1 for usage, 2 for cannot open first file, 3 for cannot open second file
 */
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

  exit(0);
}

/** @brief compare_two_files: compares two files line by line, printing the first line in which they differ
 *
 * @param first_file    first file to compare
 * @param second_file   second file to compare
 * @param argv          argument vector from main so we know the program's name and file names
 *
 */
void compare_two_files(FILE *first_file, FILE *second_file, char *argv[]) {
  unsigned int largest_file_name_size = strlen(argv[1]);
  if (strlen(argv[2]) > largest_file_name_size)
    largest_file_name_size = strlen(argv[2]);

  char line_from_first_file[MAX_LINE_LENGTH], line_from_second_file[MAX_LINE_LENGTH];
  int line_number = 0;

  // While true, keep reading and comparing lines. The loop breaks when either
  // file reaches end of file or a difference in line is found
  while (1) {
    int read_line_from_first_file = fgets(line_from_first_file, MAX_LINE_LENGTH, first_file) != NULL;
    int read_line_from_second_file = fgets(line_from_second_file, MAX_LINE_LENGTH, second_file) != NULL;
    line_number++;

	// Stop searching if both reads were EOF
    if (!read_line_from_first_file && !read_line_from_second_file)
      break;

	// Break if a file ends before the other
    if (!read_line_from_first_file && read_line_from_second_file) {
      // *s means pad the string with variable number of spaces
      // We give it the largest file name so it only pads the shorter one
      printf("%*s @ line %d: <<reached end of file>>\n", largest_file_name_size, argv[1], line_number);
      printf("%*s @ line %d: %s", largest_file_name_size, argv[2], line_number, line_from_second_file);

      break;
    }
    else if (read_line_from_first_file && !read_line_from_second_file) {
      printf("%*s @ line %d: %s", largest_file_name_size, argv[1], line_number, line_from_first_file);
      printf("%*s @ line %d: <<reached end of file>>\n", largest_file_name_size, argv[2], line_number);

      break;
    }
    if (compare_lines(line_from_first_file, line_from_second_file, argv, line_number, largest_file_name_size) == 0) {
      return;
    }
  }
}

/** @brief compare_lines: compare two lines, returning 1 if same and 0 if not same
 *
 * @param first_line              first line to compare
 * @param second_line             second line to compare
 * @param argv                    argument vector from main to know the program's name and file names
 * @param line_number             line number for printing diffs
 * @param largest_file_name_size  size of largest file name so we pad the shorter one with spaces
 *
 * @return   1 if lines are the same and 0 if not the same
 */
int compare_lines(char *first_line, char *second_line, char *argv[], int line_number, unsigned int largest_file_name_size) {

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
    // *s means pad the string with variable number of spaces
    // We give it the largest file name so it only pads the shorter one
    printf("%*s @ line %d: %s", largest_file_name_size, argv[1], line_number, first_line);
    printf("%*s @ line %d: %s\n", largest_file_name_size, argv[2], line_number, second_line);

    return 0;
  }

  return 1;
}
