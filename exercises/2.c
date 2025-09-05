#include <unistd.h>
#include <stdio.h>


int main() {
    char *header = "HTTP/1.1 200 OK";
    write(1, header, 15);
    printf("\n\nThat output was from write(). This is from printf: %s\n", header);
    return 0;
}
