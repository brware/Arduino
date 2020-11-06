//------------------------------------------------------------------------------------------
//
//  Classe de para funções e Um Mutex basico
//
//-------------------------------------------------------------------------------------------

#ifndef brwMutex_h
#define brwMutex_h

#include <Arduino.h>



class brwMutex
{
    public:

        brwMutex ();
        bool takeMutex (); // vai usar o sistema de arquivos 
        bool giveMutex (); // vai usar o sistema de arquivos
        bool waitMutex (int tmp_ms); // aguarda liberar mutex em milesegundos, 0 (Zero) guarda para sempre
        

    private:

        bool myMutex; // se esta usando o sietema de arquivos
        
};

#endif
