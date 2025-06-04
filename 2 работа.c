#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    
    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Ошибка создания сокета \n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Преобразование IP-адреса из текстового в бинарный формат
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nНеверный адрес/ Адрес не поддерживается \n");
        return -1;
    }
    
    // Подключение к серверу
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nОшибка подключения \n");
        return -1;
    }
    
    // Формирование данных студента
    char student_info[BUFFER_SIZE];
    printf("Введите данные студента (ФИО группа курс): ");
    fgets(student_info, BUFFER_SIZE, stdin);
    student_info[strcspn(student_info, "\n")] = 0; // Удаление символа новой строки
    
    // Отправка данных серверу
    send(sock, student_info, strlen(student_info), 0);
    printf("Данные студента отправлены\n");
    
    // Чтение ответа от сервера
    read(sock, buffer, BUFFER_SIZE);
    printf("Ответ сервера: %s\n", buffer);
    
    // Закрытие сокета
    close(sock);
    
    return 0;
}