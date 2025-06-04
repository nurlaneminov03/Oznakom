#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define PORT 8080
#define BUFFER_SIZE 4096

void send_404(int client_fd) {
    const char* response =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html><body><h1>404 Not Found</h1></body></html>";
    send(client_fd, response, strlen(response), 0);
}

void send_image(int client_fd, const char* message) {
    FILE* img = fopen("mirea.png", "rb");
    if (!img) {
        send_404(client_fd);
        return;
    }

    struct stat st;
    stat("mirea.png", &st);
    int img_size = st.st_size;

    char header[BUFFER_SIZE];
    snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html><body>"
        "<h1>%s</h1>"
        "<img src='data:image/png;base64,", message);

    send(client_fd, header, strlen(header), 0);

    unsigned char* img_data = malloc(img_size);
    fread(img_data, 1, img_size, img);
    fclose(img);

    const char* base64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    for (int i = 0; i < img_size; i += 3) {
        unsigned char input[3] = {0,0,0};
        int len = img_size - i >= 3 ? 3 : img_size - i;
        memcpy(input, img_data + i, len);

        char encoded[5];
        encoded[0] = base64_table[(input[0] >> 2) & 0x3F];
        encoded[1] = base64_table[((input[0] & 0x03) << 4) | ((input[1] >> 4) & 0x0F)];
        encoded[2] = (len > 1) ? base64_table[((input[1] & 0x0F) << 2) | ((input[2] >> 6) & 0x03)] : '=';
        encoded[3] = (len > 2) ? base64_table[input[2] & 0x3F] : '=';
        encoded[4] = '\0';

        send(client_fd, encoded, 4, 0);
    }

    free(img_data);

    const char* footer = "'/></body></html>";
    send(client_fd, footer, strlen(footer), 0);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    listen(server_fd, 10);
    printf("Сервер запущен на порту %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, sizeof(buffer) - 1);

        char* get = strstr(buffer, "GET /?message=");
        if (get) {
            char* msg_start = get + strlen("GET /?message=");
            char* msg_end = strchr(msg_start, ' ');
            if (msg_end) *msg_end = '\0';

            for (char* p = msg_start; *p; ++p)
                if (*p == '+') *p = ' ';

            send_image(client_fd, msg_start);
        } else {
            send_404(client_fd);
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
