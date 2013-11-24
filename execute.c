#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>

#include "cpu.h"

int main(int argc, char** argv) {
  cpu_t cpu;
  struct stat fileinfo;
  unsigned char* image = NULL;
  int fd, rc, nread;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <program.bin>\n", argv[0]);
    return -1;
  }

  fprintf(stderr, "> loading program image from `%s'\n", argv[1]);

  fd = open(argv[1], O_RDONLY);
  rc = fstat(fd, &fileinfo);

  fprintf(stderr, "> program image is %lld bytes\n", fileinfo.st_size);

  image = malloc(fileinfo.st_size);

  do {
    rc = read(fd, image + nread, fileinfo.st_size - nread);

    if (rc == -1) {
      return -1;
    }

    nread += rc;
  } while (rc != 0);

  fprintf(stderr, "> read program\n");

  cpu_init(&cpu);
  cpu.image_size = nread;
  cpu.image = image;
  cpu_run(&cpu);

  return 0;
}
