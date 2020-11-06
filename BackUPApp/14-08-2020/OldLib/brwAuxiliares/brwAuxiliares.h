//------------------------------------------------------------------------------------------
//
//  Classe de funções auxiliares de transformação de variaveis
//
//-------------------------------------------------------------------------------------------

#ifndef brwAuxiliares_h
#define brwAuxiliares_h
 
#include <Arduino.h>

class brwAuxiliares
{
    public:
        brwAuxiliares();
        void substring(char s[], char sub[], int p, int l);
        bool atoh(uint8_t * myuint,  char * mystring);
        String i64ToString(uint64_t x);
        void toFormatoIP (uint8_t * fIP, char * c_str);
        void IpToStr (char * c_str, uint8_t * fIP); 
        void rtox0 (char * c_str);  
        String removeRfinal (String str1);
        
 
    private:
        
};

#endif
