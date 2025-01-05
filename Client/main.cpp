#include "Client.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "rus");
    std::cout << "Client was started\n";

    // Проверяем, передали ли IP-адрес в качестве аргумента командной строки
    if (argc != 2)
    {
        std::cerr << "Usage: client <IP_address>\n";
        return 1; // Возвращаем код ошибки, если аргумент не передан
    }

    const char *ipAddr = argv[1];  // Получаем IP-адрес из аргумента командной строки
    int sock, port = 5175;         // Определяем переменные для сокета и порта сервера
    struct sockaddr_in servAddres; // Определяем структуру для адреса сервера
    char buffer[256];              // Буфер для хранения сообщений

    // Проверяем корректность формата IP-адреса
    if (inet_pton(AF_INET, ipAddr, &servAddres.sin_addr) <= 0)
    {
        // Обработка ошибок преобразования IP-адреса
        if (errno == EAFNOSUPPORT)
        {
            std::cerr << "Error: Invalid IP address format\n";
        }
        else
        {
            std::cerr << "Error: inet_pton failed \n";
        }
        return 1;
    }

    // Создаем сокет
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "Error: opening socket \n";
        return 1;
    }

    // Заполняем структуру адреса сервера нулями
    memset(&servAddres, 0, sizeof(servAddres));
    servAddres.sin_family = AF_INET; // Используем IPv4
    // Преобразуем IP-адрес из текстового формата в бинарный и сохраняем в структуру
    if (inet_pton(AF_INET, ipAddr, &servAddres.sin_addr) <= 0)
    {
        std::cerr << "Error: pton \n";
        close(sock); // Закрываем сокет при ошибке
        return 1;
    }
    servAddres.sin_port = htons(port); // Устанавливаем порт сервера, преобразуя его из host byte order в network byte order

    // Пытаемся установить соединение с сервером
    if (connect(sock, (struct sockaddr *)&servAddres, sizeof(servAddres)) < 0)
    {
        std::cerr << "Error: connecting \n";
        close(sock); // Закрываем сокет при ошибке
        return 1;
    }

    // Выводим приветственное сообщение и инструкцию по использованию калькулятора
    std::cout << "Добро пожаловать в Калькулятор!\n"
              << "Введите выражение в формате: a1_+_a2 либо a1_+_(a2_+_a3), где переменная обязательно число,\n"
              << "при вычислении 3 переменных необходимо проставить минимум одну открывающую и закрывающую скобки, и между знаком и числом необходимо ставить пробел,\n"
              << "Доступные операции: +,-,*,/ \n "
              << "Если вы хотите выйти из калькулятора, введите вместо выражения кодовое слово \"exit\" \n";

    // Основной цикл работы клиента
    while (true)
    {
        // Вызываем функцию function для получения выражения от пользователя
        if (function(buffer))
        {
            // Если пользователь ввел "exit", выходим из цикла
            if (strcmp(buffer, "exit") == 0)
                break;

            // Отправляем выражение на сервер
            send(sock, buffer, strlen(buffer), 0);

            // Получаем ответ от сервера
            ssize_t bytes_received = recv(sock, buffer, 255, 0);
            if (bytes_received > 0)
            {
                buffer[bytes_received] = '\0';                  // Добавляем нуль-терминатор к полученным данным
                std::cout << "Result: " << buffer << std::endl; // Выводим результат вычисления
            }
            else if (bytes_received == 0)
            {
                std::cout << "Server closed the connection." << std::endl; // Сообщение об отключении сервера
                break;
            }
            else
            {
                std::cerr << "Error receiving data." << std::endl; // Выводим ошибку, если не удалось получить ответ от сервера
                break;
            }
        }
    }

    close(sock); // Закрываем сокет при завершении работы клиента
    return 0;
}