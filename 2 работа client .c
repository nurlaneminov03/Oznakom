#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char message[] = "Эминов Нурлан Шахин оглы ККСО-26-24 1 курс";

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Неверный адрес");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при подключении");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if (send(sock_fd, message, strlen(message), 0) < 0) {
        perror("Ошибка при отправке сообщения");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Сообщение отправлено серверу: %s\n", message);

    close(sock_fd);

    return 0;
}
