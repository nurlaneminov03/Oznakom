
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080  // Порт для прослушивания
#define PORT 8080  // Порт для cервера
#define BUFFER_SIZE 256  // Размер буфера

int main() {
    int server_fd, client_socket;  // Дескрипторы сокетов
    struct sockaddr_in address;  // Структура адреса
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};  // Буфер для данных

    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    address.sin_family = AF_INET;  // IPv4
    address.sin_addr.s_addr = INADDR_ANY;  // Все интерфейсы
    address.sin_port = htons(PORT);  // Указанный порт

    // Привязка сокета к порту
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Ошибка привязки");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Ожидание подключений
    if (listen(server_fd, 3) < 0) {
        perror("Ошибка ожидания");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Сервер запущен на порту %d...\n", PORT);

    // Принятие подключения
    if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Ошибка принятия");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Клиент подключен!\n");

    // Чтение данных от клиента
    int valread = read(client_socket, buffer, BUFFER_SIZE);
    printf("Получено: %.*s\n", valread, buffer);  // Вывод полученных данных

    // Закрытие соединений
    close(client_socket);
    close(server_fd);
    return 0;
}
