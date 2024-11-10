#include "Client.h"



bool formatChecker(char* buffer) {
string temp = buffer, buffer1, buffer2, buffer3, buffer4, buffer5, trash;
bool check = false;


if(temp.find('(') == string::npos) {
  
  std::istringstream iss(temp);
  std::getline(iss, buffer1, ' '); 
  std::getline(iss, buffer2, ' '); 
  std::getline(iss, buffer3, '\0');
if(buffer2 == "+" || buffer2 == "-" || buffer2 == "*" || buffer2 == "/" || buffer2 == "") {
    if(buffer1.find(' ') == std::string::npos) {
        if(buffer3.find(' ') == std::string::npos) check = true;
        else cout << "Вторая переменная не удовлетворяет формату\n";
    }
    else cout << "Первая переменная не удовлетворяет формату\n";
    }
    else cout << "Знак операции не удовлетворяет формату\n";

memset(buffer, 0 , sizeof(buffer));
    int index = temp.find(' ');
    temp.erase(index);
    int index = temp.find(' ');
    temp.erase(index);
    
    for(int i = 0; i < temp.length(); i++) {
buffer[i] = temp[i];
    }
    
  return check;
}

else {
 
  if(temp.find(' ') == string::npos) {
  int open_parentheses = 0;
  for (char c : temp) {
    if (c == '(') {
      open_parentheses++;
    } else if (c == ')') {
      open_parentheses--;
      if (open_parentheses < 0) {
        
        return check;
      }
    }
  }
  if (open_parentheses != 0) {
    return check;
  }
check = true;
  }
  else cout << "Наличие пробелов не удовлетворяет данному формату, внимательно ознакомьтесь с инструкцией.\n";


memset(buffer, 0 , sizeof(buffer));

    int index1 = temp.find('(');
    temp.erase(index1);
    int index2 = temp.find(')');
    temp.erase(index2);
    if(index1 < 2) {for(int i = 0; i < temp.length(); i++) {
      buffer[i] = temp[i];
    }}
    else {
      int i = 0;
      while(index1+ 1 != temp.length()) {
        buffer[i] = temp[index1 + i];
        i++;
      }
      int j = 0;
      while(i != temp.length()) {
        buffer[i] = temp[j];
        i++;
        j++;
      }

    }
    
  return check;
}


}



bool function (char* buffer) {

cin.getline(buffer, 256);
if(formatChecker(buffer)) {

  return true;
  }
else return false;

}