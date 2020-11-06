//------------------------------------------------------------------------------------------
//
//  Classe de para funções basicas de arquivo 
//
//-------------------------------------------------------------------------------------------

#ifndef brwSpiffs_h
#define brwSpiffs_h
 
#include <FS.h>   
#include "SPIFFS.h"

#include <Arduino.h>
#include "brwMutex.h"

class brwSpiffs : private brwMutex
{
    public:
        bool isOpen;

        brwSpiffs ();
        bool beginFS(void);
        void closeFS(void);

        void formatFS(void);
        
        bool createFile(const char * arqname);
        bool deleteFile(const char * path);
        bool renameFile(const char * path1, const char * path2);
        void listDir( const char * dirname, uint8_t levels);
        bool escreveLinha (File *f, const char * linha);
        bool leLinha (File *f, char * linha);


    private:

        bool openFS(void);
        
};

#endif
