
#include "brwAuxiliares.h"

//-------------------------------------------------------------------------------------------
//
// Construtor da classe -> não faz nada
//
//-------------------------------------------------------------------------------------------

brwAuxiliares::brwAuxiliares()
{
   
}


//-------------------------------------------------------------------------------------------
//
// Função de substring de uma string 
// 		s-> string completa
// 		sub -> (sub)string qeue será retornada
//		p -> inicio
//		l -> final
//
//-------------------------------------------------------------------------------------------

void brwAuxiliares::substring(char s[], char sub[], int p, int l) {
   int c = 0;
   
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}


//------------------------------------------------------------------------------------------
//
// Função de char para (decimal) Hexa
//
//------------------------------------------------------------------------------------------

bool brwAuxiliares::atoh(uint8_t * myuint,  char * mystring)
{   
 int i, j; 
for (i = 0; i < (strlen(mystring) / 2); i++) {
        myuint[i] = 0;
        for (j = 0; j < 2; j++) {
            char firstchar = mystring[(i*2)+j];
            //printf("myuint[%d] = %3d mystring[%d+%d] = %c ", i, myuint[i], i, j, mystring[(i*2)+j]);
            if (firstchar >= '0' && firstchar <= '9') {
                //Serial.print("Number");
                myuint[i] = myuint[i]*16 + firstchar - '0';
            } else if (firstchar >= 'A' && firstchar <= 'F') {
                //Serial.print("LETTER");
                myuint[i] = myuint[i]*16 + firstchar - 'A' + 10;
            } else if (firstchar >= 'a' && firstchar <= 'f') {
                //Serial.print("letter");
                myuint[i] = myuint[i]*16 + firstchar - 'a' + 10;
            } else {
                // error
                //Serial.println("NOOOO");
                return (false);
            }
            //printf(" myuint[%d] = %3d\n", i, myuint[i]);
        }
    }
    return (true);
}

//------------------------------------------------------------------------------------------
//
// uint64_t para String
//
//------------------------------------------------------------------------------------------

String brwAuxiliares::i64ToString(uint64_t x)
{
     boolean flag = false; // For preventing string return like this 0000123, with a lot of zeros in front.
     String str = "";      // Start with an empty string.
     uint64_t y = 10000000000000000000;
     int res;
     if (x == 0)  // if x = 0 and this is not testet, then function return a empty string.
     {
           str = "0";
           return str;  // or return "0";
     }   
     while (y > 0)
     {               
            res = (int)(x / y);
            if (res > 0)  // Wait for res > 0, then start adding to string.
                flag = true;
            if (flag == true)
                str = str + String(res);
            x = x - (y * (uint64_t)res);  // Subtract res times * y from x
            y = y / 10;                   // Reducer y with 10   
     }
     return str;
} 

//------------------------------------------------------------------------------------------
//
//   String separada por "sep" para IP
//
//------------------------------------------------------------------------------------------

void brwAuxiliares::toFormatoIP (uint8_t * fIP, char * c_str) {

  char * p;
  uint8_t uint;
  int i;
  
  
   
  p = strtok( (char *) c_str, ".");
   
  uint = atoi (p);
  fIP[0] = uint;

  p  = (char *) strtok(NULL, ".");
  uint = atoi (p);
  fIP[1] = uint;
 
  p  = (char *) strtok(NULL, ".");
  uint = atoi (p);
  fIP[2] = uint;
  
  p  = (char *) strtok(NULL, ".");
  uint = atoi (p);
  fIP[3] = uint;


/*
  Serial.println ("IP decomposto... ");
  for (i = 0; i < 4; i++)
    Serial.println (fIP[i]);
*/
  
}





//------------------------------------------------------------------------------------------
//
//    IP para String
//
//------------------------------------------------------------------------------------------


void brwAuxiliares::IpToStr (char * c_str, uint8_t * fIP) {

  sprintf (c_str, "%d.%d.%d.%d",fIP[0], fIP[1],fIP[2],fIP[3]);


  
}


//------------------------------------------------------------------------------------------
//
//------------------  RETIRA '\r' DO FINAL DA STRING E COLOCA UM '\0'
//
//------------------------------------------------------------------------------------------

void brwAuxiliares::rtox0 (char * c_str) {
  int i;

  for (i = 0; i < strlen (c_str); i++)
      if (c_str[i] == '\r')
         c_str[i] = '\0'; 
       
}




//------------------------------------------------------------------------------------------
//
//------------------  RETIRA '\r' DO FINAL DA STRING 
//
//------------------------------------------------------------------------------------------

String brwAuxiliares::removeRfinal (String str) {
 int idx;

 idx = str.indexOf('\r');
 if (idx > 0)
   str.remove (idx);
 
 return (str);

}

//------------------------------------------------------------------------------------------
//
//------------------  LIMPA STRING COLOCANDO 0 EM TODOS CHAR
//
//------------------------------------------------------------------------------------------

void brwAuxiliares::StrClear(char *str, char length)  
 {  
  for (int i = 0; i < length; i++) {  
   str[i] = 0;  
  }  
 } 

//------------------------------------------------------------------------------------------
//
//------------------  VERIFICA SE SFIND EXISTE EM STR
//
//------------------------------------------------------------------------------------------
bool  brwAuxiliares::StrContains(char *str, char *sfind)  
 {  
  char found = 0;  
  char index = 0;  
  char len;  
  len = strlen(str);  
  if (strlen(sfind) > len) {  
   return false;  
  }  
  while (index < len) {  
   if (str[index] == sfind[found]) {  
    found++;  
    if (strlen(sfind) == found) {  
     return true;  
    }  
   }  
   else {  
    found = false;  
   }  
   index++;  
  }  
  return false;  
 }  