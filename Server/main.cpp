
#include "Server.h"




int main () {
    setlocale(LC_ALL, "rus");
cout << "Server was started\n";
int sock, newSock, port = 5175;
socklen_t cliLength;
char buffer[256];
struct sockaddr_in servAddres, cliAddres;



sock = socket(AF_INET, SOCK_STREAM, 0); // create socket
if(sock < 0) {
    cout << " Error: opening socket \n";
    exit(1);
}

memset(&servAddres, 0, sizeof(servAddres));
servAddres.sin_family = AF_INET;
servAddres.sin_addr.s_addr = INADDR_ANY;
servAddres.sin_port = htons(port);

if(bind(sock, (struct sockaddr *) &servAddres, sizeof(servAddres)) < 0) { // privyazka socketa k adresu and porty
    cout << " Error: building \n";
    exit(1);
}

listen(sock, 5);

cliLength = sizeof(cliAddres);

newSock = accept(sock, (struct sockaddr *) &cliAddres, &cliLength);
if(newSock < 0) {
    cout << "Error: accept \n";
    exit(1);
}

while(true) {
    recv(newSock, buffer, 255, 0);

function(buffer);

send(newSock, buffer, strlen(buffer), 0);
}


close(newSock);
close(sock);

cout << "Server was killed\n";
    return 0;
}