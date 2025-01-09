
#include <iostream>     // Для ввода/вывода в консоль (std::cout, std::cerr)
#include <sys/socket.h> // Для работы с сокетами (socket(), bind(), listen(), accept())
#include <netinet/in.h> // Для структур адресов интернета (sockaddr_in)
#include <arpa/inet.h>  // Для преобразования IP-адресов (inet_addr(), inet_ntoa())
#include <string>       // Для работы со строками (strcpy(), strlen(), strcmp())
#include <vector>       // Для работы с динамическими массивами (std::vector)
#include <unistd.h>     // Для различных системных функций (close())
#include <cstdlib>      // Для общих утилит (exit(), atoi())
#include <sstream>      // Для работы с потоками строк (std::stringstream)
#include <cerrno>       // Для обработки ошибок (errno)
#include <regex>        // Для работы с регулярными выражениями (std::regex, std::regex_match())
#include <fcntl.h>      // Для работы с файловыми дескрипторами (fcntl(), O_NONBLOCK)
#include <chrono>       // Для работы с временем
#include <thread>       // Для работы с потоками
#include <unistd.h>     // Для sleep()

// Типы сообщений
enum MessageType
{
    NONE = 0x00,    // Нет сообщения
    REQUEST = 0x01, // Запрос
    ACK = 0x02,     // Подтверждение
    RESULT = 0x03   // Результат
};

// Прототип функции hello
void hello();

// Объявление внешней функции, которая обрабатывает буфер и возвращает bool
// Принимает указатель на строку для возможности её модификации
bool function(std::string &buffer);

// Объявление внешней функции, которая проверяет формат буфера и возвращает bool
// Принимает константную ссылку на строку, так как не должна менять содержимое
bool formatChecker(const std::string &buffer);

// Функция для отправки сообщения (с добавлением типа и длины)
ssize_t sendMessage(int sock, const std::string &payload, MessageType type, const struct sockaddr_in &addr);

// Функция для получения сообщения
std::pair<MessageType, std::string> receiveMessage(int sock, struct sockaddr_in &clientAddr, socklen_t &clientAddrLen);