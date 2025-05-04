#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define COPYBUFFERSIZE 1
/* cat: concatenate files, version 2 */
int main(int argc, char *argv[])
{
    int fd;
    void filecopy(int, int);

    char *prog = argv[0]; /* program name for errors */
    if (argc == 1 ) /* no args; copy standard input */
        filecopy(STDIN_FILENO, STDOUT_FILENO);
    else
        while (--argc > 0)
            if ((fd = open(*++argv, O_RDONLY)) < 0) {
                fprintf(stderr, "%s: can't open %s\n",
                prog, *argv);
                return 1;
            } else {
                filecopy(fd, STDOUT_FILENO);
                if (close(fd) == -1) {
                    perror("close");
                }
            }
    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        return 2;
    }
    return 0;
}

/* filecopy: copy file ifp to file ofp */
void filecopy(int fd1, int fd2) {
    char buffer[COPYBUFFERSIZE];
    ssize_t num_read;

    // 0 indicates end of file for read syscall, despite EOF being -1, which is error for read
    while ((num_read = read(fd1, buffer, COPYBUFFERSIZE)) != 0) {

        if (num_read < 0) {
            perror("read");
            return;
        }

        ssize_t num_written = write(fd2, buffer, num_read);

        if (num_written < 0) {
            perror("write");
            return;
        }
    }

}
