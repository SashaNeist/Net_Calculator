
#include "Server.h"

int main()
{
    setlocale(LC_ALL, "rus");
    std::cout << "Server was started\n";

    int tcpSock, udpSock, port = 5175;        // Объявляем переменные для сокета и порта сервера.
    socklen_t cliLength;                      // Объявляем переменную для хранения размера структуры адреса клиента.
    struct sockaddr_in servAddres, cliAddres; // Объявляем структуры для адресов сервера и клиента.
    pthread_t thread_id;                      // Объявляем переменную для идентификатора потока.

    // Создаем сокет
    tcpSock = socket(AF_INET, SOCK_STREAM, 0); // Создаем сокет для TCP-соединения.
    if (tcpSock < 0)                           // Проверяем, что сокет был создан успешно
    {
        std::cout << " Error: opening socket \n"; // Выводим сообщение об ошибке создания сокета.
        exit(1);
    }

    udpSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSock < 0)
    {
        std::cerr << "Error: opening UDP socket\n";
        close(tcpSock);
        return 1;
    }

    // Устанавливаем опцию SO_REUSEADDR, которая позволяет переиспользовать адрес
    int reuse = 1; // Значение опции (1 - включить)
    if (setsockopt(tcpSock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        std::cerr << "Error setting SO_REUSEADDR option\n"; // Выводим сообщение об ошибке установки опции.
        close(tcpSock);                                     // Закрываем сокет.
        close(udpSock);
        return 1;
    }
    if (setsockopt(udpSock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        std::cerr << "Error setting SO_REUSEADDR option\n"; // Выводим сообщение об ошибке установки опции.
        close(tcpSock);                                     // Закрываем сокет.
        close(udpSock);
        return 1;
    }

    memset(&servAddres, 0, sizeof(servAddres)); // Заполняем структуру адреса сервера нулями.
    servAddres.sin_family = AF_INET;            // Указываем семейство адресов (IPv4).
    servAddres.sin_addr.s_addr = INADDR_ANY;    // Указываем, что сервер принимает соединения на всех интерфейсах.
    servAddres.sin_port = htons(port);          // Устанавливаем порт сервера, преобразуя его из host byte order в network byte order.

    // Связываем TCP сокет с адресом
    if (bind(tcpSock, (struct sockaddr *)&servAddres, sizeof(servAddres)) < 0)
    {                                       // Проверяем, удалось ли связать сокет с адресом.
        std::cout << " Error: building \n"; // Выводим сообщение об ошибке при связывании сокета.
        exit(1);
    }
    // Связываем UDP сокет с адресом
    if (bind(udpSock, (struct sockaddr *)&servAddres, sizeof(servAddres)) < 0)
    {
        std::cerr << "Error: binding UDP socket\n";
        close(tcpSock);
        close(udpSock);
        return 1;
    }
    // Переводим сокет в режим прослушивания, разрешаем до 5 ожидающих соединений.
    if (listen(tcpSock, 5) < 0)
    {
        std::cerr << "Error: listening on TCP socket\n";
        close(tcpSock);
        close(udpSock);
        return 1;
    }
    // Переводим UDP сокет в неблокирующий режим
    int flagsUDP = fcntl(udpSock, F_GETFL, 0);
    if (flagsUDP == -1)
    {
        perror("fcntl F_GETFL failed");
        close(tcpSock);
        close(udpSock);
        return 1;
    }
    if (fcntl(udpSock, F_SETFL, flagsUDP | O_NONBLOCK) == -1)
    {
        perror("fcntl F_SETFL failed");
        close(tcpSock);
        close(udpSock);
        return 1;
    }
    cliLength = sizeof(cliAddres); // Получаем размер структуры адреса клиента.
    fd_set read_fds;               // Создаем множество дескрипторов для чтения
    // Главный цикл приема соединений
    while (true) // Бесконечный цикл для приема новых соединений.
    {

        FD_ZERO(&read_fds);         // Очищаем множество дескрипторов
        FD_SET(tcpSock, &read_fds); // Добавляем TCP сокет в множество для чтения
        FD_SET(udpSock, &read_fds); // Добавляем UDP сокет в множество для чтения

        // Определяем максимальный файловый дескриптор, который будет использован в select
        // Прибавляем 1, т.к. select использует значения меньше чем max_fd
        int max_fd = std::max(tcpSock, udpSock) + 1;

        // Вызываем select, чтобы дождаться готовности хотя бы одного из сокетов к чтению
        // Первый аргумент - максимальный дескриптор + 1
        // Второй аргумент - указатель на множество дескрипторов для чтения
        // Третий и четвертый аргументы - для записи и исключений (здесь nullptr, не используются)
        // Пятый аргумент - тайм-аут (здесь nullptr, т.е. select будет ждать бесконечно)
        if (select(max_fd, &read_fds, nullptr, nullptr, nullptr) < 0)
        {
            // Если select возвращает -1, значит произошла ошибка
            perror("select failed"); // Выводим сообщение об ошибке
            continue;                // Переходим к следующей итерации цикла
        }

        // TCP
        if (FD_ISSET(tcpSock, &read_fds))
        {
            int *newSock_ptr = new int;                                                // Выделяем динамически память для хранения сокета клиента.
            *newSock_ptr = accept(tcpSock, (struct sockaddr *)&cliAddres, &cliLength); // Принимаем новое соединение от клиента.
            // Проверяем, удалось ли принять соединение
            if (*newSock_ptr < 0)
            {
                std::cout << "Error: accept \n"; // Выводим сообщение об ошибке при принятии соединения.
                delete newSock_ptr;              // Освобождаем память в случае ошибки.
                continue;                        // Продолжаем слушать новые соединения.
            }

            // Создаем поток для обработки клиента
            if (pthread_create(&thread_id, NULL, handle_client, (void *)newSock_ptr) != 0)
            {                                                      // Проверяем, удалось ли создать поток.
                std::cerr << "Error creating thread" << std::endl; // Выводим сообщение об ошибке при создании потока.
                close(*newSock_ptr);                               // Закрываем сокет клиента.
                delete newSock_ptr;                                // Освобождаем память выделенную под сокет.
                continue;                                          // Продолжаем слушать новые соединения.
            }
            pthread_detach(thread_id); // Освобождаем ресурсы потока после завершения его работы.
        }
        // UDP
        else if (FD_ISSET(udpSock, &read_fds))
        {
            // Если в наборе дескрипторов read_fds есть udp сокет, то есть пришли данные по udp
            struct sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);

            // Пытаемся получить UDP сообщение и его тип от клиента
            std::pair<MessageType, std::string> received = receiveUdpMessage(udpSock, clientAddr, clientAddrLen);
            // Если тип сообщения REQUEST
            if (received.first == MessageType::REQUEST)
            {
                // Отправляем клиенту подтверждение (ACK) о получении запроса
                if (sendUdpMessage(udpSock, "", MessageType::ACK, clientAddr, clientAddrLen) < 0)
                {
                    std::cerr << "Error send UDP ACK\n";
                    continue; // Если ошибка отправки ACK, переходим к следующей итерации ожидания сообщений
                }

                // Обрабатываем полученные данные и получаем результат
                std::string result = function(received.second);
                // Отправляем клиенту результат обработки
                if (sendUdpMessage(udpSock, result, MessageType::RESULT, clientAddr, clientAddrLen) < 0)
                {
                    std::cerr << "Error send UDP RESULT\n";
                    continue; // Если ошибка отправки RESULT, переходим к следующей итерации ожидания сообщений
                }
                // Ожидание ACK от клиента на отправленный результат
                bool askReceived = false;
                // Фиксируем время начала ожидания
                auto startTime = std::chrono::steady_clock::now();
                while (true)
                {
                    // Получаем текущее время
                    auto currentTime = std::chrono::steady_clock::now();
                    // Вычисляем длительность ожидания в секундах
                    auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
                    // Проверяем, истек ли таймаут ожидания ACK (3 секунды)
                    if (duration >= 3)
                    {
                        std::cout << "Timeout waiting for Result UDP ACK\n";
                        break; // Если таймаут истек, выходим из цикла ожидания ACK
                    }
                    // Пытаемся получить UDP сообщение, ожидая ACK от клиента на результат
                    std::pair<MessageType, std::string> receivedAsk = receiveUdpMessage(udpSock, clientAddr, clientAddrLen);
                    // Если полученное сообщение типа ACK
                    if (receivedAsk.first == MessageType::ACK)
                    {
                        askReceived = true;
                        break; // Выходим из цикла ожидания ACK, так как он получен
                    }
                    // Пауза на 10 миллисекунд
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                // Если ACK не был получен в течение таймаута
                if (!askReceived)
                    std::cout << "Потеряна связь с UDP клиентом\n";
                // Вывод сообщения об ошибке, если получен неизвестный тип сообщения
                else if (received.first != MessageType::ACK && !received.second.empty())
                    std::cerr << "Unknow message type received: " << static_cast<int>(received.first) << std::endl;
            }
        }
    }

    close(tcpSock); // Закрываем TCP сокет
    close(udpSock); // Закрываем UDP сокет
    std::cout << "Server was killed\n";
    return 0;
}