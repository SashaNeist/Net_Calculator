#include "Client.h"

void hello()
{
  // Выводим приветственное сообщение и инструкцию по использованию калькулятора
  std::cout << "Добро пожаловать в Калькулятор!\n"
            << "Введите выражение в формате: a1+a2 либо a1+(a2+a3), где переменная обязательно число,\n"
            << "Доступные операции: +,-,*,/ \n"
            << "Если вы хотите выйти из калькулятора, введите вместо выражения кодовое слово \"exit\" \n";
}
// Функция `formatChecker` проверяет формат ввода пользователя.
// Возвращает `true`, если формат ввода корректен, `false` в противном случае.
bool formatChecker(const std::string buffer)
{
  std::string temp = buffer; // Создаем объект string

  // Регулярное выражение для выражений вида "число оператор число".
  std::regex pattern_2nums(R"(^\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*([0-9]+(?:\.[0-9]+)?)\s*$)");

  // Регулярное выражение для выражений вида "число оператор (число оператор число)".
  std::regex pattern_3nums_right(R"(^\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*\(\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*([0-9]+(?:\.[0-9]+)?)\s*\)\s*$)");

  // Регулярное выражение для выражений вида "(число оператор число) оператор число".
  std::regex pattern_3nums_left(R"(^\s*\(\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*([0-9]+(?:\.[0-9]+)?)\s*\)\s*([\+\-\*\/])\s*([0-9]+(?:\.[0-9]+)?)\s*$)");

  // Регулярное выражение для выражений вида "число оператор число оператор число".
  std::regex pattern_3nums_2(R"(^\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*([0-9]+(?:\.[0-9]+)?)\s*$)");

  // Проверяем, соответствует ли ввод одному из шаблонов.
  if (std::regex_match(temp, pattern_2nums) || std::regex_match(temp, pattern_3nums_right) || std::regex_match(temp, pattern_3nums_left) || std::regex_match(temp, pattern_3nums_2) || temp == "exit")
  {
    if (temp == "exit")
      std::cout << "Received exit command. Client closed.\n";
    return true;
  }
  else
  {
    // Выводим сообщение об ошибке, если ввод не соответствует ожидаемому формату.
    std::cerr << "Неверный формат ввода. Ожидается 'число оператор число' либо 'число оператор (число оператор число)' либо 'число оператор число оператор число' либо команда 'exit'.\n";
    return false;
  }
}

// Функция `function` считывает ввод пользователя и вызывает `formatChecker` для проверки формата.
bool function(std::string *buffer)
{
  // Читаем строку из стандартного ввода в буфер.
  std::cin >> *buffer;
  // Проверяем формат ввода.
  if (formatChecker(*buffer))
  {
    return true;
  }
  else
    return false;
}

// Функция для отправки сообщения (с добавлением типа и длины)
ssize_t sendMessage(int sock, const std::string &payload, MessageType type, const struct sockaddr_in &addr)
{

  std::string message;
  message += static_cast<char>(type);                          // Добавляем тип сообщения в начало строки
  uint16_t payloadLen = static_cast<uint16_t>(payload.size()); // Получаем длину полезной нагрузки
  message += static_cast<char>((payloadLen >> 8) & 0xFF);      // Добавляем длину полезной нагрузки (2 байта) в message: // старший байт
  message += static_cast<char>(payloadLen & 0xFF);             // младший байт
  message += payload;                                          // Добавляем саму полезную нагрузку в конец строки

  // Отправляем сообщение через сокет, используя sendto
  // Возвращаем количество отправленных байтов или -1 при ошибке
  return sendto(sock, message.c_str(), message.size(), 0, (const sockaddr *)&addr, sizeof(addr));
}

// Функция для получения сообщения
std::pair<MessageType, std::string> receiveMessage(int sock, struct sockaddr_in &clientAddr, socklen_t &clientAddrLen)
{
  // Буфер для всего сообщения
  char buffer[65535];
  // Получение данных из сокета, используя recvfrom
  ssize_t bytes_received = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);

  // Обработка ошибок
  if (bytes_received == -1)
  {
    // Проверяем, что ошибка связана с отсутствием данных или блокировкой
    if (errno == EAGAIN || errno == EWOULDBLOCK)
    {
      std::cerr << "Error: Data is empty\n";
      return {MessageType::NONE, ""}; // Возвращаем NONE, если нет данных
    }
    else
    {
      // Если ошибка другая
      std::cerr << "Error: Unknown error - " << strerror(errno) << "\n"; // Выводим сообщение об ошибке с описанием
      return {MessageType::NONE, ""};                                    // Возвращаем NONE, если произошла ошибка
    }
  }

  // Проверяем, что сообщение имеет минимальную длину (1 байт для типа + 2 байта для длины)
  if (bytes_received < 3)
  {
    std::cerr << "Error: Received message too short\n";
    return {MessageType::NONE, ""}; // Если сообщение слишком короткое, то возвращаем NONE
  }

  MessageType type = static_cast<MessageType>(buffer[0]);                                           // Получение типа сообщения
  uint16_t payloadLen = (static_cast<uint16_t>(buffer[1]) << 8) | static_cast<uint16_t>(buffer[2]); // Получение длины полезной нагрузки

  // Проверка соответствия полученной длины и длины из сообщения
  if (bytes_received - 3 != payloadLen)
  {
    std::cerr << "Error: Received data length mismatch\n";
    return {MessageType::NONE, ""}; // Возвращаем NONE, если есть несоответствие
  }

  std::string payload(buffer + 3, buffer + 3 + payloadLen); // Создание строки payload из полученных данных
  return {type, payload};                                   // Возвращаем тип сообщения и полезную нагрузку
}
