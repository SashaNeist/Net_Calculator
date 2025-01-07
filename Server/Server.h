
#include <iostream>     // Для ввода/вывода в консоль (std::cout, std::cerr)
#include <sys/socket.h> // Для работы с сокетами (socket(), bind(), listen(), accept())
#include <netinet/in.h> // Для структур адресов интернета (sockaddr_in)
#include <arpa/inet.h>  // Для преобразования IP-адресов (inet_addr(), inet_ntoa())
#include <string.h>     // Для работы со строками (strcpy(), strlen(), strcmp())
#include <unistd.h>     // Для различных системных функций (close())
#include <cstdlib>      // Для общих утилит (exit(), atoi())
#include <sstream>      // Для работы с потоками строк (std::stringstream)
#include <regex>        // Для работы с регулярными выражениями (std::regex, std::regex_match())
#include <pthread.h>    // Для работы с потоками (pthread_create(), pthread_join(), pthread_exit())
#include <fcntl.h>      // Для работы с файловыми дескрипторами (fcntl(), O_NONBLOCK)
#include <chrono>       // Для работы со временем
#include <thread>       // Для работы с потоками

// Типы сообщений (из клиентского кода)
enum MessageType
{
    NONE = 0x00,    // Нет сообщения
    REQUEST = 0x01, // Запрос
    ACK = 0x02,     // Подтверждение
    RESULT = 0x03   // Результат
};

// Объявление внешней функции, которая будет обрабатывать полученную строку
std::string function(std::string buffer);

// Объявление функции, которая будет обрабатывать клиента в отдельном потоке
void *handle_client(void *arg);

// Функция для отправки сообщения по UDP
ssize_t sendUdpMessage(int sock, const std::string &payload, MessageType type, const struct sockaddr_in &addr, socklen_t addrLen);

// Функция для получения сообщения по UDP
std::pair<MessageType, std::string> receiveUdpMessage(int sock, struct sockaddr_in &clientAddr, socklen_t &clientAddrLen);

// Объявления математических функций для операций с числами double
double addition(const double &d1, const double &d2);       // Сложение
double deduction(const double &d1, const double &d2);      // Вычитание
double multiplication(const double &d1, const double &d2); // Умножение
double separation(const double &d1, const double &d2);     // Деление