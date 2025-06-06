#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080  // Порт для подключения

int main() {
    int sock;  // Дескриптор сокета
    struct sockaddr_in serv_addr;  // Структура адреса сервера
    
    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    serv_addr.sin_family = AF_INET;  // Семейство адресов IPv4
    serv_addr.sin_port = htons(PORT);  // Порт сервера
    
    // Преобразование IP-адреса
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Ошибка в IP-адресе");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Подключение к серверу
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Ошибка подключения");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Отправка данных серверу
    char* message = "Эминов Нурлан Шахин оглы ККСО-26-24 1 курс";
    send(sock, message, strlen(message), 0);
    printf("Сообщение отправлено\n");

    // Закрытие соединения
    close(sock);
    return 0;
}
