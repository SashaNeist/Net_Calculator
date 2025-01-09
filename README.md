          For launch Net_Calculator you need:
Compile Server: "g++ -pthread -o Server Server.cpp main.cpp"
Compile Client: "g++ -o Client Client.cpp main.cpp"
Launch Server: "./Server"
Launch Client: "./Client <IP-Адрес сервера> <Протокол>"

При запуске Сервера и клиент на одной машине используется адрес: "127.0.0.1"
Виды протоколов: 
1. tcp - Протокол сетевых взаимодействий TCP/IP
2. udp - Протокол сетевых взаимодействий UDP

          Для запуска теста необходимо:

Launch Server
Launch script.exp: "chmod +x script.exp" "./script.exp"

Тестовый bash-скрипт: test.sh на данный момент не работает.
