#include "domoticaTi.h"

void removeBadCharacters(char* data) {
  int dataLength = strlen(data);
  char *res = malloc(dataLength * sizeof(char) + 10);
  char *out = res;
  for(int i = 0; data[i] != '\0' && i < dataLength; i++) {
    if((data[i] >= 'a' && data[i] <= 'z') || (data[i] >= 'A' && data[i] <= 'Z') || (data[i] >= '0' && data[i] <= '9')) {
      *out++ = data[i];
    } else {
      switch (data[i]) {
        case '-':
          *out++ = data[i];
        break;
      
        case '_':
          *out++ = data[i];
        break;
      
        case '.':
          *out++ = data[i];
        break;
        
        case ',':
          *out++ = data[i];
        break;
        
        case '+':
          *out++ = data[i];
        break;
        
        case '*':
          *out++ = data[i];
        break;
            
        case '@':
          *out++ = data[i];
        break;
            
        case '!':
          *out++ = data[i];
        break;
            
        case '?':
          *out++ = data[i];
        break;
            
        case '#':
          *out++ = data[i];
        break;
            
        case '%':
          *out++ = data[i];
        break;
            
        case '&':
          *out++ = data[i];
        break;
            
        case '(':
          *out++ = data[i];
        break;
            
        case ')':
          *out++ = data[i];
        break;
            
        case '/':
          *out++ = data[i];
        break;
            
        case ':':
          *out++ = data[i];
        break;
            
        case ' ':
          *out++ = data[i];
        break;
            
      default:
        break;
      }
    }
    
  }
  *out++ = '\0';
  strcpy(data, res);
}