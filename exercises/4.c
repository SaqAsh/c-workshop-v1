#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

const char *DEFAULT_FILE = "index.html";

char *to_path(char *req) {
  char *start, *end;

  for (start = req; start[0] != ' '; start++) {
    if (!start[0]) {
      return NULL;
    }
  }

  start++;

  for (end = start; end[0] != ' '; end++) {
    if (!end[0]) {
      return NULL;
    }
  }

  if (end[-1] != '/') {
    end[0] = '/';
    end++;
  }

  if (end + strlen(DEFAULT_FILE) > req + strlen(req)) {
    return NULL;
  }

  memcpy(end, DEFAULT_FILE, strlen(DEFAULT_FILE) + 1);

  return start + 1;
}

void print_file(const char *path) {
  int fd = open(path, O_RDONLY);

  if (fd == -1) {
    perror("Failed to open file");
    exit(EXIT_FAILURE);
  }

  struct stat metadata;
  fstat(fd, &metadata);

  char *buf = (char *)malloc(metadata.st_size + 1);

  if (buf == NULL) {
    close(fd);
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_read = read(fd, buf, metadata.st_size);

  if (bytes_read == -1) {
    close(fd);
    free(buf);
    perror("Failed to read bytes");
    exit(EXIT_FAILURE);
  }

  buf[bytes_read] = '\0';

  printf("\n%s contents:\n\n%s\n", path, buf);

  free(buf);
  close(fd);
}

int main() {
  char req1[] = "GET / HTTP/1.1\nHost: example.com";
  print_file(to_path(req1));

  char req2[] = "GET /blog HTTP/1.1\nHost: example.com";
  print_file(to_path(req2));

  return 0;
}
