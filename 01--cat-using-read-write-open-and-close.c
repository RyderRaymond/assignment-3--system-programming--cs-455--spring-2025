#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// Set to 1 as getc reads 1 character in the original program
#define COPY_BUFFER_SIZE 1

void filecopy(int, int);

/* cat: concatenate files, version 2 (with syscalls) */
int main(int argc, char *argv[])
{
    int file_descriptor;
    char *prog = argv[0]; /* program name for errors */

    if (argc == 1 ) /* no args; copy standard input */
        filecopy(STDIN_FILENO, STDOUT_FILENO);
    else {
        while (--argc > 0) {
            if ((file_descriptor = open(*++argv, O_RDONLY)) < 0)
            {
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

/* filecopy: copy file ifp to file ofp */
void filecopy(const int file_descriptor_in, const int file_descriptor_out) {
    char buffer[COPY_BUFFER_SIZE];
    ssize_t num_bytes_read;

    // 0 indicates end of file for the read syscall, despite EOF being -1, which is an error for the read syscall
    while ((num_bytes_read = read(file_descriptor_in, buffer, COPY_BUFFER_SIZE)) != 0) {

        if (num_bytes_read < 0) {
            perror("Error reading file");
            return;
        }

        ssize_t num_written = write(file_descriptor_out, buffer, num_bytes_read);

        if (num_written < 0) {
            perror("Error writing file contents");
            return;
        }
    }

}
