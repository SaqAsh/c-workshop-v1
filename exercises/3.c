#include <stdio.h>
#include <string.h>

const char *DEFAULT_FILE = "index.html";

char *to_path(char *req) {
  char *start, *end;
  // this parses the GET, PUT, POST, etc...
  for (start = req; start[0] != ' '; start++) {
    if (!start[0]) {
      return NULL;
    }
  }

  start += 2; // Eat the space as well as the /

  // This parses the /main or /blog of the request
  // Advance end to the second space
  for (end = start; end[0] != ' '; end++) {
    if (!end[0]) {
      return NULL;
    }
  }

  // this adds the / after the blog so we get something like blog/index.html
  if ((*(end - 1) != '/')) {
    *end = '/';
    end++;
  }
  // if we don't have enough room to copy the default file, return NULL
  if (end + strlen(DEFAULT_FILE) > req + strlen(req)) {
    return NULL;
  }

  // copy the file path using memcpy
  memcpy(end, DEFAULT_FILE, strlen(DEFAULT_FILE) + 1);

  return start;
}

int main() {
  char req1[] = "GET /blog HTTP/1.1\nHost: example.com";
  printf("Should be \"blog/index.html\": \"%s\"\n", to_path(req1));

  char req2[] = "GET /blog/ HTTP/1.1\nHost: example.com";
  printf("Should be \"blog/index.html\": \"%s\"\n", to_path(req2));

  char req3[] = "GET / HTTP/1.1\nHost: example.com";
  printf("Should be \"index.html\": \"%s\"\n", to_path(req3));
  // 👉 Finally, fix it by handling the case where `req` is too short to
  //    have "index.html" memcpy'd into it.
  //    Hint: `strlen()` returns an integer whose type is not `int` but rather
  //    `size_t`
  char req4[] = "GET /blog ";
  printf("Should be \"(null)\": \"%s\"\n", to_path(req4));

  return 0;
}
