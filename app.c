#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 4096

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    ssize_t valread;

    // Set up the socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind to port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return -1;
    }

    printf("Listening on port %d\n", PORT);

    // Block until we get a connection on that socket
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        return -1;
    }

    // Read all the bytes from the socket into the buffer
    valread = read(new_socket, buffer, BUFFER_SIZE);

    // Parse the HTTP request
    char *method, *url;
    method = strtok(buffer, " ");
    if (method) {
        url = strtok(NULL, " ");
        if (url) {
            printf("Received request for: %s\n", url);

            // Translate the URL into a file path, e.g. "/foo" becomes "./foo/index.html"
            char file_path[1024];
            snprintf(file_path, sizeof(file_path), ".%s/index.html", url);

            // Open and read the file
            int file_fd = open(file_path, O_RDONLY);
            if (file_fd < 0) {
                perror("file open");
                close(new_socket);
                close(server_fd);
                return -1;
            }

            char file_buffer[BUFFER_SIZE];
            ssize_t file_read = read(file_fd, file_buffer, BUFFER_SIZE);
            close(file_fd);

            // Send the contents of the file as the HTTP response
            char response_header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
            write(new_socket, response_header, strlen(response_header));
            if (file_read > 0) {
                write(new_socket, file_buffer, file_read);
            }
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
