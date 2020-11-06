
#include "brwSpiffs.h"

//------------------------------------------------------------------------------------------
//
//------------------  CONSTRUTOR DA CLASSE
//
//------------------------------------------------------------------------------------------

brwSpiffs::brwSpiffs() {

}

//------------------------------------------------------------------------------------------
//
//------------------  FORMATA O 'DISCO'
//
//------------------------------------------------------------------------------------------

void brwSpiffs::formatFS(void){
  Serial.println ("Formatando sistema de arquivos");
  SPIFFS.format();
  Serial.println ("Formatou sistema de arquivos");
}

//------------------------------------------------------------------------------------------ 
//
//------------------  ABRE O SISTEMA DE ARQUIVOS
//
//------------------------------------------------------------------------------------------

bool brwSpiffs::openFS(void) {
  //Abre o sistema de arquivos
  if(!SPIFFS.begin(true)) // true -> format on fail
    return (false);
  else 
    return (true);

  
}


//------------------------------------------------------------------------------------------ 
//
//------------------  FECHA O SISTEMA DE ARQUIVOS
//
//------------------------------------------------------------------------------------------

void brwSpiffs::closeFS(void) {
  //Abre o sistema de arquivos
  SPIFFS.end();
  
}

//------------------------------------------------------------------------------------------
//
//------------------  CRIA ARQUIVO NO MODO WRITE, SE EXISTIR APAGA
//
//------------------------------------------------------------------------------------------

bool brwSpiffs::createFile(const char * arqname) {
  File wFile;
 

  //Cria o arquivo se ele não existir
  if(SPIFFS.exists(arqname)) { //Arquivo ja existe
    Serial.println ("Arquivo Existe");
    return (false);
  } else { //Criando o arquivo.
        wFile = SPIFFS.open(arqname,FILE_WRITE); //Verifica a criação do arquivo
         if(!wFile) {
            Serial.println ("ERRO ao criar Arquivo"); 
            return (false);//Erro ao criar arquivo
         }
          
  }
  wFile.close();
  return (true); //Arquivo criado com sucesso!
}
 
//------------------------------------------------------------------------------------------
//
//------------------  APAGA ARQUIVO
// 
//------------------------------------------------------------------------------------------


bool brwSpiffs::deleteFile(const char * path) {
  
    
    if(SPIFFS.remove(path))
        return (true); // Arquivo apagado
    else 
        return (false); //Falha ao apagar arquivo
    
}



//------------------------------------------------------------------------------------------
//
//------------------  RENOMEIA ARQUIVO
//
//------------------------------------------------------------------------------------------


bool brwSpiffs::renameFile(const char * path1, const char * path2) {
  
    
    if (SPIFFS.rename(path1, path2)) 
        return (true);
    else 
        return (false);

    
}//------------------------------------------------------------------------------------------
//
//------------------  LISTA OS DIRETORIOS
//
//------------------------------------------------------------------------------------------


void brwSpiffs::listDir( const char * dirname, uint8_t levels) {
    
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = SPIFFS.open(dirname);
    if(!root){
        Serial.println("- Falha ao abrir diretorio");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - Não é um diretorio");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}




