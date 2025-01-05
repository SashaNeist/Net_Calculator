
#include <iostream>     // Для ввода/вывода в консоль (std::cout, std::cerr)
#include <sys/socket.h> // Для работы с сокетами (socket(), bind(), listen(), accept())
#include <netinet/in.h> // Для структур адресов интернета (sockaddr_in)
#include <arpa/inet.h>  // Для преобразования IP-адресов (inet_addr(), inet_ntoa())
#include <string.h>     // Для работы со строками (strcpy(), strlen(), strcmp())
#include <vector>       // Для работы с динамическими массивами (std::vector)
#include <unistd.h>     // Для различных системных функций (close())
#include <cstdlib>      // Для общих утилит (exit(), atoi())
#include <sstream>      // Для работы с потоками строк (std::stringstream)
#include <cerrno>       // Для обработки ошибок (errno)
#include <regex>        // Для работы с регулярными выражениями (std::regex, std::regex_match())

// Объявление внешней функции, которая обрабатывает буфер и возвращает bool
bool function(char *buffer);

// Объявление внешней функции, которая проверяет формат буфера и возвращает bool
bool formatChecker(const char *buffer);