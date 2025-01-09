#!/bin/bash

SERVER_IP="127.0.0.1" #IP-адрес сервера
TIMEOUT = 2           #Тайм-аут ожидания ответа

tests=(
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

./Server
SERVER_PID=$!
sleep 1

run_test() {
    input=$1
    expected=$2

    echo -n "$input" | nc -w $TIMEOUT $SERVER_IP >result.txt
    result=$(cat result.txt)

    if [["$result" == "$expected"]]; then
        echo "Test Passed:  $input = $result"
        echo ""
    else
        echo "Test Failed: $input (expected $expected, got $result)"
        echo ""
    fi
}

for test in "${tests[@]}"; do
    input=$(echo $test | awk 'print $1')
    expected=$(echo $test | awk '{print $2}')
    run_test "$input" "$expected"
done
#Остановка сервера
kill $SERVER_PID
