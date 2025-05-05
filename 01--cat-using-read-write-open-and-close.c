#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// Set to 1 as getc reads 1 character in the original program
#define COPY_BUFFER_SIZE 1

void filecopy(int, int);

/** @brief cat: concatenate files, version 2 (with syscalls)
 *
 *  Experiments:
 *
 *  To test the speed of this versus the chapter's version, a 60KB file was passed
 *  as the argument to this version and the chapter's version. Time was recorded with
 *  the "time" Unix command.
 *
 *  Speed of original version using std file functions:
 *    real    0m0.011s
 *    user    0m0.002s
 *    sys     0m0.009s
 *
 *  Speed of this version using system calls:
 *    real    0m0.355s
 *    user    0m0.021s
 *    sys     0m0.329s
 *
 *  @param argc number of arguments cat is called with
 *  @param argv the actual vector or array of arguments
 *  @return status of cat, with 0 being success and !0 error
 */
int main(int argc, char *argv[])
{
  int file_descriptor;
  char *prog = argv[0]; /* program name for errors */

  if (argc == 1 ) /* no args; copy standard input */
    filecopy(STDIN_FILENO, STDOUT_FILENO);
  else {
    while (--argc > 0) {
      if ((file_descriptor = open(*++argv, O_RDONLY)) < 0) {
        fprintf(stderr, "%s: can't open %s: ", prog, *argv);
        perror(""); //empty string as the fprintf in the previous line does the formatting

        return 1;
      }
      else {
        filecopy(file_descriptor, STDOUT_FILENO);

        if (close(file_descriptor) < 0) {
          fprintf(stderr, "%s: %s: ",
          prog, *argv);
          perror("Error closing file");
        }
      }
    }
  }
  if (ferror(stdout)) {
    fprintf(stderr, "%s: error writing stdout\n", prog);

    return 2;
  }

  return 0;
}

/** @brief filecopy: copy from one file descriptor to another
 *
 *  @param file_descriptor_in   the file descriptor to read in text from
 *  @param file_descriptor_out  the file descriptor to write out to
 */
void filecopy(const int file_descriptor_in, const int file_descriptor_out) {
  char buffer[COPY_BUFFER_SIZE];
  ssize_t num_bytes_read;

  // 0 indicates end of file for the read syscall, despite EOF being -1, which is an error for the read syscall
  while ((num_bytes_read = read(file_descriptor_in, buffer, COPY_BUFFER_SIZE)) != 0) {
    if (num_bytes_read < 0) {
      perror("Error reading file");

      return;
    }

    ssize_t num_bytes_written = write(file_descriptor_out, buffer, num_bytes_read);

    if (num_bytes_written < 0) {
      perror("Error writing file contents");

      return;
    }
  }
}
