#include "Client.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "rus");

    // Проверяем, передали ли IP-адрес в качестве аргумента командной строки
    if (argc != 3)
    {
        std::cerr << "Usage: client <IP_address> <tcp/udp>\n";
        return 1; // Возвращаем код ошибки, если аргумент не передан
    }

    const char *ipAddr = argv[1];  // Получаем IP-адрес из аргумента командной строки
    const char *netProt = argv[2]; // Получаем выбранный пользователем сетевой протокол
    int sock, port = 5175;         // Определяем переменные для сокета и порта сервера
    struct sockaddr_in servAddres; // Определяем структуру для адреса сервера
    std::string buffer;            // Буфер для хранения сообщений

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
    std::cout << "Client was started. Connected with " << netProt << std::endl;

    if (!std::strcmp(netProt, "tcp")) // Проверяем выбранный протокол передачи данных
    {                                 // Соединение с помощью TCP

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
        hello();

        // Основной цикл работы клиента
        while (true)
        {
            // Вызываем функцию function для получения выражения от пользователя
            if (function(&buffer))
            {
                // Если пользователь ввел "exit", выходим из цикла
                if (buffer == "exit")
                    break;

                // Отправляем выражение на сервер

                send(sock, buffer.c_str(), buffer.size(), 0);

                // Получаем ответ от сервера
                char temp_buffer[256];
                ssize_t bytes_received = recv(sock, temp_buffer, 255, 0);
                if (bytes_received > 0)
                {
                    buffer.assign(temp_buffer, bytes_received);
                    if (buffer == "Division by 0")
                        std::cerr << "Received separation Error: Деление на 0\n";

                    else
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
    }

    else // Соединение с помощью UDP
    {

        // Создаем UDP сокет
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0)
        {
            std::cerr << "Error: opening UDP socket" << std::endl;
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
                                           // Переводим сокет в неблокирующий режим
        int flags = fcntl(sock, F_GETFL, 0);
        if (flags == -1)
        {
            perror("fcntl F_GETFL failed");
            close(sock);
            return 1;
        }
        if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1)
        {
            perror("fcntl F_SETFL failed");
            close(sock);
            return 1;
        }

        hello(); // Выводим приветственное сообщение и инструкцию по использованию калькулятора
        socklen_t servAddresLen = sizeof(servAddres);
        // Основной цикл работы клиента
        while (true)
        {
            // Вызываем функцию function для получения выражения от пользователя
            if (function(&buffer))
            {
                // Проверяем, если пользователь ввел команду "exit"
                if (buffer == "exit")
                    break; // Завершаем цикл, если введена команда "exit"

                int retryCount = 0;       // Счетчик повторных отправок сообщения
                bool ackReceived = false; // Флаг, показывающий, был ли получен ACK (подтверждение)

                // Цикл повторных отправок, пока не будет получено подтверждение (ACK)
                // или не будет достигнуто максимальное количество попыток
                while (retryCount < 3 && !ackReceived)
                {
                    // Отправляем выражение на сервер
                    ssize_t bytesSend = sendMessage(sock, buffer, MessageType::REQUEST, servAddres);
                    // Проверяем, успешно ли отправлено сообщение
                    if (bytesSend < 0)
                    {
                        std::cerr << "Error: sendto failed\n";
                        close(sock);
                        return 1; // Возвращаем код ошибки, если отправка не удалась
                    }

                    // Ждем ACK (подтверждение) от сервера
                    auto startTime = std::chrono::steady_clock::now(); // Запоминаем время начала ожидания

                    // Цикл ожидания ACK, пока не истечет таймаут или не будет получен ACK
                    while (true)
                    {
                        // Получаем текущее время
                        auto currentTime = std::chrono::steady_clock::now();
                        // Вычисляем длительность ожидания
                        auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
                        // Проверяем, истек ли таймаут (3 секунды)
                        if (duration >= 3)
                        {
                            std::cout << "Timeout waiting for ACK\n";
                            break; // Выход из цикла ожидания ACK (по таймауту)
                        }
                        // Пытаемся получить сообщение (ACK или другой ответ) от сервера
                        std::pair<MessageType, std::string> received = receiveMessage(sock, servAddres, servAddresLen);
                        // Проверяем, получили ли мы ACK

                        if (received.first == MessageType::ACK)
                        {
                            ackReceived = true; // Устанавливаем флаг, что ACK получен
                            break;              // Выход из цикла ожидания ACK (ACK получен)
                        }
                        // Пауза на 10 миллисекунд перед следующей попыткой чтения
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                    // Если ACK не был получен
                    if (!ackReceived)
                        retryCount++; // Увеличиваем счетчик повторных отправок
                }

                // Проверяем, был ли получен ACK после всех попыток
                if (!ackReceived)
                {
                    std::cout << "Connecting lost\n";
                    break; // Выход из основного цикла (потеря связи)
                }
                // Если ACK получен, то продолжаем работу
                else
                {
                    // Получаем ответ с сервера
                    std::pair<MessageType, std::string> receivedResult;

                    // Цикл ожидания сообщения с результатом
                    while (true)
                    {
                        // Получаем сообщение от сервера
                        receivedResult = receiveMessage(sock, servAddres, servAddresLen);
                        // Проверяем, является ли сообщение результатом вычисления
                        if (receivedResult.first == MessageType::RESULT)
                            break; // Выход из цикла ожидания (результат получен)
                        // Пауза перед следующей попыткой чтения
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                    // Обработка ошибки: Деление на 0
                    if (receivedResult.second == "Division by 0")
                        std::cerr << "Received separation Error: Деление на 0\n";
                    // Выводим результат
                    else
                        std::cout << "Result: " << receivedResult.second << std::endl;

                    // Отправляем ACK на результат
                    if (sendMessage(sock, "", MessageType::ACK, servAddres) < 0)
                        std::cerr << "Error sending result ask\n";
                }
            }
        }
        close(sock); // Закрываем сокет при завершении работы клиента
    }

    return 0;
}