#include "Client.h"


int main (const char* ipAddr) {
    setlocale(LC_ALL, "rus");

cout << "Client was started\n";
int sock, port = 5175;
struct sockaddr_in servAddres;
char buffer[256];

sock = socket(AF_INET, SOCK_STREAM, 0);
if(sock < 0) {
    cout << "Error: opening socket \n";
    exit(1);
}

memset(&servAddres, 0, sizeof(servAddres));
servAddres.sin_family = AF_INET;
if (inet_pton(AF_INET, ipAddr, &servAddres.sin_addr) <= 0) {
  cout << " Error: pton \n";
  exit(1);
}
servAddres.sin_port = htons(port);

if(connect(sock, (struct sockaddr *) &servAddres, sizeof(servAddres)) < 0) {
    cout << " Error: connecting \n";
    exit(1);
}

cout << "Добро пожаловать в Калькулятор!\n" << "Введите выражение в формате: a1 + a2, где переменная обязательно число,\n"
<< "при вычислении 2 переменных между символами необходимо ставить пробел\n"
<< "при вычислении 3 переменных необходимо проставить минимум одну открывающую и закрывающую скобки и НЕ ставить пробелы между символами,\n"
<< "максимальное кол-во чисел = 3, разрешено использование скобок.\n"
  << "В имени переменной не должно быть пробелов. Доступные операции: +,-,*,/ \n "
 <<"Если вы хотите выйти из калькулятора, введите вместо выражения кодовое слово \"exit\" \n";
while(true) {
    if(function(buffer)) {
        if (strcmp(buffer, "exit") == 0) break;

    send(sock, buffer, strlen(buffer), 0);

    recv(sock, buffer, 255, 0);


    cout << "Result: " << buffer << endl;
    }

    
}

close(sock);


    return 0;
}