
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

// Объявление внешней функции, которая будет обрабатывать полученную строку
void function(char *buffer);

// Объявление функции, которая будет обрабатывать клиента в отдельном потоке
void *handle_client(void *arg);

// Объявления математических функций для операций с числами double
double addition(const double &d1, const double &d2);
double deduction(const double &d1, const double &d2);
double multiplication(const double &d1, const double &d2);
double sepatation(const double &d1, const double &d2);