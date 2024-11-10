#include "Server.h"

double addition (const double& d1, const double& d2) {return d1+d2;}
double  deduction (const double& d1, const double& d2) {return d1-d2;}
double  multiplication (const double& d1, const double& d2) {return d1*d2;}
double  sepatation (const double& d1, const double& d2) { if(d2 == 0) {cout << "Error: Деление на \"0\" не предусмотрено\n"; return 999999; } else return d1/d2;}


void function (char* buffer) {
  cout << "Buffer: " << buffer << endl;
    string temp = buffer, char1, char2, char3, sign1, sign2;
    if()
    memset(buffer, 0 , sizeof(buffer));
  std::istringstream iss(temp);
  cout << "Separation of buffer\n";
  std::getline(iss, char1, ' '); 
  cout << char1 << "\n";
  std::getline(iss, sign1, ' '); 
  cout << sign1 << "\n";
  std::getline(iss, char2, ' ');
  cout << char2 << "\n";
char sign = sign1[0];
double result;
double d1,d2;
d1 = stod(char1);
d2 = stod(char2);
  switch(sign) {
case '+': result = addition(d1, d2); break;
case '-': result = deduction(d1, d2); break;
case '*': result = multiplication(d1, d2); break;
case '/': result = sepatation(d1, d2); break;
  }

  temp = to_string(result);
  strcpy(buffer, temp.c_str());
  cout << "Перевод string temp to char* buffer прошел успешно\n";

}

