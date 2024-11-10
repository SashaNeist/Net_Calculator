#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <sstream>
using namespace std;

void function (char* buffer);

double addition (const double& d1, const double& d2);
double  deduction (const double& d1, const double& d2); 
double  multiplication (const double& d1, const double& d2); 
double  sepatation (const double& d1, const double& d2); 