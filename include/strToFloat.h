#ifndef STRTOFLOAT_H
#define STRTOFLOAT_H

#include <string>

float strToFloat(std::string inStr)
{
  int signVal = 1;
  if(inStr.substr(0, 1).find("-") != std::string::npos){
    signVal = -1;
    inStr.replace(0, 1, "");
  }
  
  std::string eStr = "";

  if(inStr.find("E") != std::string::npos){
    eStr = inStr.substr(inStr.find("E"), inStr.size() - inStr.find("E"));
    inStr.replace(inStr.find("E"), inStr.size() - inStr.find("E"), "");
  }

  float base = std::stof(inStr);

  if(eStr.size() != 0){
    bool doMult = true;
    if(eStr.substr(1, 1).find("-") != std::string::npos) doMult = false;
    float exp = std::stoi(eStr.substr(2, eStr.size()-2));
    while(exp > 0){
      if(doMult) base *= 10;
      else base /= 10;
      
      exp--;
    }
  }

  return base*signVal;
}

#endif
