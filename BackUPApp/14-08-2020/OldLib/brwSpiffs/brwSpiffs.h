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

class brwSpiffs
{
    public:
        brwSpiffs ();
        void formatFS(void);
        bool openFS(void);
        void closeFS(void);
        bool createFile(const char * arqname);
        bool deleteFile(const char * path);
        bool renameFile(const char * path1, const char * path2);
        void listDir( const char * dirname, uint8_t levels);

    private:
        
};

#endif
