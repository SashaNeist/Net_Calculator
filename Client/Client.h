#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <string.h>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <sstream>

using namespace std;


bool function (char* buffer);
bool formatChecker(char* buffer);