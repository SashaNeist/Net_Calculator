#include "Client.h"

// Функция `formatChecker` проверяет формат ввода пользователя.
// Возвращает `true`, если формат ввода корректен, `false` в противном случае.
bool formatChecker(const char *buffer)
{
  std::string temp = buffer; // Создаем объект string из переданного массива символов.

  // Регулярное выражение для двух чисел.
  std::regex pattern_2nums(R"(^\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*([0-9]+(?:\.[0-9]+)?)\s*$)");

  // Регулярное выражение для трех чисел со скобками.
  std::regex pattern_3nums(R"(^\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*\(\s*([0-9]+(?:\.[0-9]+)?)\s*([\+\-\*\/])\s*([0-9]+(?:\.[0-9]+)?)\s*\)\s*$)");

  // Проверяем, соответствует ли ввод одному из шаблонов.
  if (std::regex_match(temp, pattern_2nums) || std::regex_match(temp, pattern_3nums) || temp == "exit")
  {
    if (temp == "exit")
      std::cout << "Received exit command. Client closed.\n";
    return true;
  }
  else
  {
    // Выводим сообщение об ошибке, если ввод не соответствует ожидаемому формату.
    std::cerr << "Неверный формат ввода. Ожидается 'число оператор число' либо 'число оператор (число оператор число)'.\n";
    return false;
  }
}

// Функция `function` считывает ввод пользователя и вызывает `formatChecker` для проверки формата.
bool function(char *buffer)
{
  // Читаем строку из стандартного ввода в буфер.
  std::cin.getline(buffer, 256);
  // Проверяем формат ввода.
  if (formatChecker(buffer))
  {
    return true;
  }
  else
    return false;
}