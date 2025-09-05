#include <stdio.h>
#include <unistd.h>
int main() {
    write(1, "Hello, World!\n", 14);
    int num = 42;
    int number = 43;
    printf("The two numbers are: %d %d\n", num, number);
    return 0;
}
