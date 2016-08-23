#ifndef GETPARTSOFSTRING_H
#define GETPARTSOFSTRING_H

#include <string>
#include <vector>

#include "include/globalDoDebug.h"

void getPartsOfString(std::string sentence, std::vector<std::string>* words_p)
{
  while(sentence.find("  ") != std::string::npos){
    sentence.erase(sentence.find("  "), 1);
  }

  while(true){
    std::size_t tempPos = sentence.find(" ");
    if(tempPos == std::string::npos){
      words_p->push_back(sentence);
      break;
    }
    
    std::string tempWord = sentence.substr(0, tempPos);

    while(tempWord.find(" ") != std::string::npos){
      tempWord.replace(tempWord.find(" "), tempWord.find(" ")+1, "");
    }

    if(tempWord.size() != 0) words_p->push_back(tempWord);

    sentence = sentence.substr(tempPos+1, sentence.size()-tempPos+1);
  }

  return;
}

#endif
