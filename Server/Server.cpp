#include "Server.h"

// Функция для сложения двух чисел типа double
double addition(const double &d1, const double &d2)
{
  return d1 + d2; // Возвращает сумму двух чисел
}
// Функция для вычитания двух чисел типа double
double deduction(const double &d1, const double &d2)
{
  return d1 - d2; // Возвращает разность двух чисел
}
// Функция для умножения двух чисел типа double
double multiplication(const double &d1, const double &d2)
{
  return d1 * d2; // Возвращает произведение двух чисел
}
// Функция для деления двух чисел типа double
double separation(const double &d1, const double &d2)
{
  if (d2 == 0) // Проверка деления на ноль
  {
    std::cerr << "Error: Деление на \"0\" не предусмотрено\n"; // Вывод сообщения об ошибке
    return 999999;
  }
  else
    return d1 / d2; // Если деление на ноль не происходит, возвращает результат деления
}

// Функция `function` выполняет разбор выражения, вычисляет результат и сохраняет его в buffer
std::string function(const std::string buffer)
{
  std::cout << "Buffer: " << buffer << std::endl; // Выводим содержимое буфера для отладки.
  std::string temp = buffer;                      // Создаем std::string из переданного буфера.

  // Регулярное выражение для выражений вида "число оператор число".
  std::regex pattern_2nums(R"(^\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*([0-9]+(?:\.[0-9]+)?)\s*$)");

  // Регулярное выражение для выражений вида "число оператор (число оператор число)".
  std::regex pattern_3nums(R"(^\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*\(\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*([0-9]+(?:\.[0-9]+)?)\s*\)\s*$)");

  // Регулярное выражение для выражений вида "число оператор (число оператор число)".
  std::regex pattern_3nums_2(R"(^\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*([0-9]+(?:\.[0-9]+)?)\s*$)");

  std::smatch matches; // Объект для хранения результатов поиска регулярного выражения
  double result = 0.0; // Инициализируем переменную для хранения результата вычисления

  // Проверяем, соответствует ли ввод шаблону с двумя числами
  if (std::regex_match(temp, matches, pattern_2nums))
  {
    // Обрабатываем выражение с двумя числами
    try
    {
      double num1 = std::stod(matches[1].str()); // Извлекаем первое число из совпадений и преобразуем в double.
      char op = matches[2].str()[0];             // Извлекаем оператор.
      double num2 = std::stod(matches[3].str()); // Извлекаем второе число и преобразуем в double.

      // Выполняем операцию в зависимости от оператора.
      switch (op)
      {
      case '+':
        result = addition(num1, num2); // Вызываем функцию addition для сложения.
        break;
      case '-':
        result = deduction(num1, num2); // Вызываем функцию deduction для вычитания.
        break;
      case '*':
        result = multiplication(num1, num2); // Вызываем функцию multiplication для умножения.
        break;
      case '/':
        result = separation(num1, num2); // Вызываем функцию sepatation для деления.
        break;
      default:
        // Если оператор не распознан, устанавливаем сообщение об ошибке и завершаем функцию
        std::cerr << "Неверная операция на сервере\n"; // Выводим сообщение об ошибке в поток cerr
        return "Неверная операция на сервере.";        // Копируем сообщение об ошибке в буфер
      }
    }
    catch (std::invalid_argument const &ex)
    {
      // Обрабатываем исключение, когда не удалось преобразовать строку в число
      std::cerr << "Неверный формат числа на сервере\n"; // Выводим сообщение об ошибке в поток cerr
      return "Неверный формат числа на сервере.";        // Копируем сообщение об ошибке в буфер
    }
    catch (std::out_of_range const &ex)
    {
      // Обрабатываем исключение, когда число выходит за диапазон допустимых значений
      std::cerr << "Число за пределами диапазона на сервере\n"; // Выводим сообщение об ошибке в поток cerr
      return "Число за пределами диапазона на сервере.";        // Копируем сообщение об ошибке в буфер
    }
  }
  // Проверяем, соответствует ли ввод шаблону с тремя числами
  else if (std::regex_match(temp, matches, pattern_3nums))
  {
    // Обрабатываем выражение с тремя числами со скобками
    try
    {
      double num1 = std::stod(matches[1].str()); // Извлекаем первое число.
      char op1 = matches[2].str()[0];            // Извлекаем внешний оператор.
      double num2 = std::stod(matches[3].str()); // Извлекаем второе число.
      char op2 = matches[4].str()[0];            // Извлекаем внутренний оператор.
      double num3 = std::stod(matches[5].str()); // Извлекаем третье число.
      double inner_result = 0.0;                 // Переменная для хранения результата вычисления во скобках

      // Выполняем операцию во скобках в зависимости от оператора.
      switch (op2)
      {
      case '+':
        inner_result = addition(num2, num3); // Вычисляем сложение чисел во скобках.
        break;
      case '-':
        inner_result = deduction(num2, num3); // Вычисляем вычитание чисел во скобках.
        break;
      case '*':
        inner_result = multiplication(num2, num3); // Вычисляем умножение чисел во скобках.
        break;
      case '/':
        inner_result = separation(num2, num3); // Вычисляем деление чисел во скобках.
        break;
      default:
        // Если оператор не распознан, устанавливаем сообщение об ошибке и завершаем функцию
        std::cerr << "Неверная опif(resul)ерация во скобках на сервере\n"; // Выводим сообщение об ошибке в поток cerr
        return "Неверная операция во скобках на сервере.";                 // Копируем сообщение об ошибке в буфер
      }

      // Выполняем внешнюю операцию.
      switch (op1)
      {
      case '+':
        result = addition(num1, inner_result); // Вычисляем сложение.
        break;
      case '-':
        result = deduction(num1, inner_result); // Вычисляем вычитание.
        break;
      case '*':
        result = multiplication(num1, inner_result); // Вычисляем умножение.
        break;
      case '/':
        result = separation(num1, inner_result); // Вычисляем деление.
        break;
      default:
        std::cerr << "Неверная внешняя операция на сервере\n"; // Выводим сообщение об ошибке в поток cerr
        return "Неверная внешняя операция на сервере.";        // Копируем сообщение об ошибке в буфер
      }
    }
    catch (std::invalid_argument const &ex)
    {
      // Обрабатываем исключение, когда не удалось преобразовать строку в число
      std::cerr << "Неверный формат числа на сервере\n"; // Выводим сообщение об ошибке в поток cerr
      return "Неверный формат числа на сервере.";        // Копируем сообщение об ошибке в буфер
    }
    catch (std::out_of_range const &ex)
    {
      // Обрабатываем исключение, когда число выходит за диапазон допустимых значений
      std::cerr << "Число за пределами диапазона на сервере\n"; // Выводим сообщение об ошибке в поток cerr
      return "Число за пределами диапазона на сервере.";        // Копируем сообщение об ошибке в буфер
    }
  }
  // Если ввод соответствует команде exit
  else if (std::regex_match(temp, matches, pattern_3nums_2))
  { // Обрабатываем выражение с тремя числами без скобок
    try
    {
      double num1 = std::stod(matches[1].str());
      char op1 = matches[2].str()[0];
      double num2 = std::stod(matches[3].str());
      char op2 = matches[4].str()[0];
      double num3 = std::stod(matches[5].str());

      if (op1 == '*' || op1 == '/')
      {
        double tempResult = (op1 == '*') ? multiplication(num1, num2) : separation(num1, num2);
        if (op2 == '+' || op2 == '-')
          result = (op2 == '+') ? addition(tempResult, num3) : deduction(tempResult, num3);
        else
          result = (op2 == '*') ? multiplication(tempResult, num3) : separation(tempResult, num3);
      }
      else if (op2 == '*' || op2 == '/')
      {
        double tempResult = (op2 == '*') ? multiplication(num2, num3) : separation(num2, num3);
        result = (op1 == '+') ? addition(num1, tempResult) : deduction(num1, tempResult);
      }
      else
      {
        double tempResult = (op1 == '+') ? addition(num1, num2) : deduction(num1, num2);
        result = (op2 == '+') ? addition(tempResult, num3) : deduction(tempResult, num3);
      }

      return std::to_string(result);
    }
    catch (std::invalid_argument const &ex)
    {
      std::cerr << "Неверный формат числа на сервере\n";
      return "Неверный формат числа на сервере.";
    }
    catch (std::out_of_range const &ex)
    {
      std::cerr << "Число за пределами диапазона на сервере\n";
      return "Число за пределами диапазона на сервере.";
    }
  }
  else if (temp == "exit")
  {

    std::cerr << "Поступила специальная команда\n"; // Выводим сообщение в поток cerr
    return "Поступила exit команда для клиента\n";
  }
  // Если ввод не соответствует ни одному из шаблонов
  else
  {

    std::cerr << "Неверный формат ввода на сервере\n"; // Выводим сообщение об ошибке в поток cerr
    return "Неверный формат ввода на сервере.";        // Копируем сообщение об ошибке в буфер
  }

  // Преобразуем результат в строку и сохраняем в буфер

  if (result == 999999) // Проверка деления на 0
  {
    std::cerr << "Received separation error: Division by 0\n";
    return "Division by 0";
  }
  else
  {
    std::string result_str = std::to_string(result);                      // Преобразуем результат в строку.
    std::cout << "Перевод string temp to string buffer прошел успешно\n"; // Выводим сообщение об успехе.
    return result_str;                                                    // Копируем результат в буфер.
  }
}
// Функция для обработки клиента в отдельном потоке
void *handle_client(void *arg)
{
  // Извлекаем дескриптор сокета клиента из аргумента
  int newSock = *(int *)arg;
  // newSock - это дескриптор сокета, который используется для обмена данными с клиентом.
  // * (int *) arg: Преобразование void* в int*, а затем разыменование указателя, чтобы получить int.

  std::string buffer; // Буфер для хранения данных от клиента

  // Бесконечный цикл для приема и обработки данных от клиента
  while (true)
  {
    char temp_buffer[256];
    ssize_t bytes_received = recv(newSock, temp_buffer, 255, 0); // Получение данных от клиента
    // recv() - получает данные из сокета.
    // newSock - дескриптор сокета.
    // buffer - буфер для хранения принятых данных.
    // 255 - максимальное количество байтов для приема.
    // 0 - флаги.
    // bytes_received - количество принятых байтов.

    // Проверка количества принятых байтов
    if (bytes_received > 0)
    {
      buffer.assign(temp_buffer, bytes_received);
      // Проверка на команду "exit"
      if (buffer == "exit")
      {
        std::cout << "Received exit command from client. Cliend closing connection.\n";
        break; // Выход из цикла, если получена команда "exit".
      }

      buffer = function(buffer); // Вызываем функцию для обработки полученных данных.

      send(newSock, buffer.c_str(), buffer.size(), 0); // Отправка данных обратно клиенту (эхо)
    }
    // Если клиент закрыл соединение
    else if (bytes_received == 0)
    {
      std::cout << "Client closed the connection." << std::endl;
      break; // Выход из цикла, если соединение закрыто клиентом.
    }
    // Обработка ошибок
    else
    {
      std::cerr << "Error receiving data.\n";
      break; // Выход из цикла, если произошла ошибка.
    }
  }

  close(newSock); // Закрытие сокета

  delete (int *)arg; // Освобождаем память, выделенную при создании потока.

  // Завершение потока
  pthread_exit(nullptr);
  // pthread_exit() - завершает текущий поток.
  // nullptr - возвращаемое значение при завершении (в данном случае, не используется).
  return nullptr;
}

// Функция для отправки сообщения по UDP
ssize_t sendUdpMessage(int sock, const std::string &payload, MessageType type, const struct sockaddr_in &addr, socklen_t addrLen)
{

  std::string message;                                         // Создаем строку для хранения сообщения
  message += static_cast<char>(type);                          // Добавляем тип сообщения в начало строки
  uint16_t payloadLen = static_cast<uint16_t>(payload.size()); // Получаем длину полезной нагрузки
  message += static_cast<char>((payloadLen >> 8) & 0xFF);      // Добавляем длину полезной нагрузки (2 байта) в message: // старший байт
  message += static_cast<char>(payloadLen & 0xFF);             // младший байт
  message += payload;                                          // Добавляем саму полезную нагрузку в конец строки

  // Отправляем сообщение через сокет, используя sendto
  // Возвращаем количество отправленных байтов или -1 при ошибке
  ssize_t bytesSent = sendto(sock, message.c_str(), message.size(), 0, (const sockaddr *)&addr, addrLen);

  if (bytesSent == -1)
  {
    std::cerr << "Error: Failed to send message\n";
  }

  return bytesSent;
}

// Функция для получения сообщения по UDP
std::pair<MessageType, std::string> receiveUdpMessage(int sock, struct sockaddr_in &clientAddr, socklen_t &clientAddrLen)
{
  // Буфер для всего сообщения
  char buffer[16383]; // Максимальный размер UDP пакета. На деле, вам может понадобится меньше
  // recvfrom() - получает сообщение с сокета, используя информацию об адресе отправителя
  ssize_t bytes_received = recvfrom(sock, buffer, sizeof(buffer), 0, (sockaddr *)&clientAddr, &clientAddrLen);

  // Обработка ошибок
  if (bytes_received == -1)
  {
    // Проверяем, что ошибка связана с отсутствием данных или блокировкой
    if (errno == EAGAIN || errno == EWOULDBLOCK)
    {
      std::cerr << "Error: Data is empty\n";
      return {MessageType::NONE, ""}; // Возвращаем NONE, если нет данных
    }
    // Если ошибка другая
    std::cerr << "Error: Unknown error - " << strerror(errno) << "\n"; // Выводим сообщение об ошибке с описанием
    return {MessageType::NONE, ""};                                    // Возвращаем NONE, если произошла ошибка
  }

  // Проверяем, что сообщение имеет минимальную длину (1 байт для типа + 2 байта для длины)
  if (bytes_received < 3)
  {
    std::cerr << "Error: Received message too short\n";
    return {MessageType::ACK, ""}; // Если сообщение слишком короткое, то возвращаем ACK
  }

  // Получение типа сообщения
  MessageType type = static_cast<MessageType>(buffer[0]); // Тип сообщения находится в первом байте

  // Получение длины полезной нагрузки (payload)
  uint16_t payloadLen = (static_cast<uint16_t>(buffer[1]) << 8) | static_cast<uint16_t>(buffer[2]); // Длина полезной нагрузки в следующих 2 байтах (в little-endian порядке)

  // Проверяем, что полученная длина соответствует фактической длине данных
  if (bytes_received - 3 != payloadLen)
  {
    std::cerr << "Error: Received data length mismatch\n";
    return {MessageType::ACK, ""}; // Возвращаем ACK, если есть несоответствие
  }

  // Создаем std::string из полезной нагрузки
  std::string payload(buffer + 3, buffer + 3 + payloadLen); // Копируем данные, начиная с 3-го байта и длиной payloadLen

  // Возвращаем тип сообщения и полезную нагрузку
  return {type, payload};
}