#!/bin/bash

# Настройка переменных
SERVER_EXEC="./Server/Server"
CLIENT_EXEC="./Client/Client"

# IP адрес сервера и протокол
SERVER_IP="127.0.0.1"
PROTOCOL="tcp"

# Массив с тестовыми данными (выражение и ожидаемый результат или "exit")
TEST_CASES=(
  "10 + 5 15"
  "20 / 4 5"
  "10 * 2 20"
  "2 + (3 + 4) 9"
  "10 - (5 - 2) 7"
  "12 * (3 / 2) 18"
  "(2 + 3) + 4 9"
  "(10 - 5) - 2 3"
  "(12 * 3) / 2 18"
  "10 + 5 * 2 20"
  "20 / 4 - 1 4"
  "2 + 3 + 4 9"
  "10 - 5 - 2 3"
  "12 * 3 / 2 18"
  "100 / 5 * 2 40"
  "1.5 + 2.5 * 2 6.5"
  "10 / 3 * 3 9.99999"
  "10 + a * 2 Неверный формат числа на сервере."
  "9999999999 + 1 * 2  Число за пределами диапазона на сервере."
  "10 + 5 +  Неверный формат выражения."
  "exit exit" # Тест для выхода
)

TEST_COUNT=0
PASS_COUNT=0
FAIL_COUNT=0

# Проверка существования исполняемых файлов
if [ ! -x "$SERVER_EXEC" ]; then
  echo "Ошибка: Исполняемый файл сервера не найден или не является исполняемым: $SERVER_EXEC"
  exit 1
fi

if [ ! -x "$CLIENT_EXEC" ]; then
  echo "Ошибка: Исполняемый файл клиента не найден или не является исполняемым: $CLIENT_EXEC"
  exit 1
fi

# Запуск сервера в фоновом режиме
echo "Запуск сервера..."
$SERVER_EXEC &
SERVER_PID=$!
sleep 2

# Цикл тестирования
for test_case in "${TEST_CASES[@]}"; do
  TEST_COUNT=$((TEST_COUNT + 1))
  IFS=' ' read -ra PARTS <<<"$test_case"
  expression="${PARTS[@]:0:${#PARTS[@]}-1}" # Все кроме последнего слова
  expected_result="${PARTS[-1]}"            # Последнее слово

  echo "Тест $TEST_COUNT: Выражение: '$expression', Ожидаемый результат: '$expected_result'"

  if [[ "$expression" == "exit" ]]; then
    echo "  Отправка exit-команды"
    actual_result=$(echo "$expression" | "$CLIENT_EXEC" "$SERVER_IP" "$PROTOCOL")
    if [[ "$actual_result" == "$expected_result" ]]; then
      echo "  Результат: '$actual_result' - Пройден"
      PASS_COUNT=$((PASS_COUNT + 1))
    else
      echo "  Результат: '$actual_result' - ПРОВАЛ (Ожидалось: '$expected_result')"
      FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
    break # Выходим из цикла, если получили exit
  else
    actual_result=$(echo "$expression" | "$CLIENT_EXEC" "$SERVER_IP" "$PROTOCOL") # Получаем вывод от клиента
    if [[ "$actual_result" == "$expected_result" ]]; then
      echo "  Результат: '$actual_result' - Пройден"
      PASS_COUNT=$((PASS_COUNT + 1))
    else
      echo "  Результат: '$actual_result' - ПРОВАЛ (Ожидалось: '$expected_result')"
      FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
  fi

done

# Убиваем процесс сервера
echo "Завершение работы сервера..."
kill $SERVER_PID

# Вывод статистики
echo "-------------------------"
echo "Общее количество тестов: $TEST_COUNT"
echo "Пройдено тестов: $PASS_COUNT"
echo "Провалено тестов: $FAIL_COUNT"

if [[ "$FAIL_COUNT" -gt 0 ]]; then
  exit 1
fi

exit 0
