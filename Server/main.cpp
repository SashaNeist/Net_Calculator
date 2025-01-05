
#include "Server.h"

int main()
{
    setlocale(LC_ALL, "rus");            
    std::cout << "Server was started\n"; 

    int sock, port = 5175;                    // Объявляем переменные для сокета и порта сервера.
    socklen_t cliLength;                      // Объявляем переменную для хранения размера структуры адреса клиента.
    struct sockaddr_in servAddres, cliAddres; // Объявляем структуры для адресов сервера и клиента.
    pthread_t thread_id;                      // Объявляем переменную для идентификатора потока.

    // Создаем сокет
    sock = socket(AF_INET, SOCK_STREAM, 0); // Создаем сокет для TCP-соединения.
    if (sock < 0)                           // Проверяем, что сокет был создан успешно
    {
        std::cout << " Error: opening socket \n"; // Выводим сообщение об ошибке создания сокета.
        exit(1);                                  
    }

    // Устанавливаем опцию SO_REUSEADDR, которая позволяет переиспользовать адрес
    int reuse = 1; // Значение опции (1 - включить)
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        std::cerr << "Error setting SO_REUSEADDR option\n"; // Выводим сообщение об ошибке установки опции.
        close(sock);                                        // Закрываем сокет.
        return 1;                                          
    }

    memset(&servAddres, 0, sizeof(servAddres)); // Заполняем структуру адреса сервера нулями.
    servAddres.sin_family = AF_INET;            // Указываем семейство адресов (IPv4).
    servAddres.sin_addr.s_addr = INADDR_ANY;    // Указываем, что сервер принимает соединения на всех интерфейсах.
    servAddres.sin_port = htons(port);          // Устанавливаем порт сервера, преобразуя его из host byte order в network byte order.

    // Связываем сокет с адресом
    if (bind(sock, (struct sockaddr *)&servAddres, sizeof(servAddres)) < 0)
    {                                       // Проверяем, удалось ли связать сокет с адресом.
        std::cout << " Error: building \n"; // Выводим сообщение об ошибке при связывании сокета.
        exit(1);                            
    }

    listen(sock, 5); // Переводим сокет в режим прослушивания, разрешаем до 5 ожидающих соединений.

    cliLength = sizeof(cliAddres); // Получаем размер структуры адреса клиента.

    // Главный цикл приема соединений
    while (true) // Бесконечный цикл для приема новых соединений.
    {
        int *newSock_ptr = new int;                                             // Выделяем динамически память для хранения сокета клиента.
        *newSock_ptr = accept(sock, (struct sockaddr *)&cliAddres, &cliLength); // Принимаем новое соединение от клиента.
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

    close(sock);                        // Закрываем серверный сокет
    std::cout << "Server was killed\n"; 
    return 0;                           
}