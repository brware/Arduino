
#include "brwSpiffs.h"


//------------------------------------------------------------------------------------------
//
//------------------  CONSTRUTOR DA CLASSE
//
//------------------------------------------------------------------------------------------

brwSpiffs::brwSpiffs() {

 isOpen = false; 

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
//------------------  INICIA SISTEMA DE ARQUIVOS
//
//------------------------------------------------------------------------------------------

bool brwSpiffs::beginFS(void) {
  bool ret;
  
  ret = openFS ();

  if (ret)
    isOpen = true;
  else
    isOpen = false;  

  return (ret);

  
}

//------------------------------------------------------------------------------------------ 
//
//------------------  ABRE O SISTEMA DE ARQUIVOS
//
//------------------------------------------------------------------------------------------

bool brwSpiffs::openFS(void) {
  //Abre o sistema de arquivos
  
  Serial.println ("Open FS");
  if(!SPIFFS.begin(true)) {  // true -> format on fail
    Serial.println ("ERRO: falha ao iniciar sistema de arquivos");
    return (false);
  }
  else { 
    Serial.println ("Iniciou sistema de arquivos");
    return (true);
  }

  
}


//------------------------------------------------------------------------------------------ 
//
//------------------  FECHA O SISTEMA DE ARQUIVOS
//
//------------------------------------------------------------------------------------------

void brwSpiffs::closeFS(void) {
  //Abre o sistema de arquivos
  SPIFFS.end();
  
  isOpen = false;
  
}

//------------------------------------------------------------------------------------------
//
//------------------  CRIA ARQUIVO NO MODO WRITE, SE EXISTIR APAGA
//
//------------------------------------------------------------------------------------------

bool brwSpiffs::createFile(const char * arqname) {
  File wFile;
 
  //takeMutex();
  //Cria o arquivo se ele não existir
  if(SPIFFS.exists(arqname)) { //Arquivo ja existe
    Serial.println ("Arquivo Existe");
    giveMutex ();
    return (false);
  } else { //Criando o arquivo.
        wFile = SPIFFS.open(arqname,FILE_WRITE); //Verifica a criação do arquivo
         if(!wFile) {
            Serial.println ("ERRO ao criar Arquivo"); 
            giveMutex ();
            return (false);//Erro ao criar arquivo
         }
          
  }
  wFile.close();
  giveMutex ();
  return (true); //Arquivo criado com sucesso!
}
 
//------------------------------------------------------------------------------------------
//
//------------------  APAGA ARQUIVO
// 
//------------------------------------------------------------------------------------------


bool brwSpiffs::deleteFile(const char * path) {
  
    takeMutex ();
    if(SPIFFS.remove(path)) {
      giveMutex ();
      return (true); // Arquivo apagado 
    }  else  {
        giveMutex ();
        return (false); //Falha ao apagar arquivo
      }
    
}



//------------------------------------------------------------------------------------------
//
//------------------  RENOMEIA ARQUIVO
//
//------------------------------------------------------------------------------------------


bool brwSpiffs::renameFile(const char * path1, const char * path2) {
  
    takeMutex ();
    if (SPIFFS.rename(path1, path2)) {
        giveMutex ();
        return (true);
    } else { 
        giveMutex ();
        return (false);
      }

    
}

//------------------------------------------------------------------------------------------
//
//------------------  LISTA OS DIRETORIOS
//
//------------------------------------------------------------------------------------------


void brwSpiffs::listDir( const char * dirname, uint8_t levels) {
    
    Serial.printf("Listando Diretorio: %s\r\n", dirname);
    
    takeMutex ();
    File root = SPIFFS.open(dirname);
    if(!root){
        Serial.println("- Falha ao abrir diretorio");
        giveMutex ();
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - Não é um diretorio");
        giveMutex ();
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
    giveMutex ();
    return;
}

//------------------------------------------------------------------------------------------
//
//------------------  Escreva uma linha no arquivo aberto f
//
//------------------------------------------------------------------------------------------

bool brwSpiffs::escreveLinha (File *f, const char * linha) {
  int num = 0;

  takeMutex ();
  f -> println (linha);
  /*
  while (num != '\n') {
    f -> write (linha[num]);
    num++;  
  }
  f -> write ('\n');
  Serial.print("Escreveu (bytes): ");
  Serial.println(num);*/
  giveMutex ();
  if (num)
    return (true);
  else
    return (false);

}


//------------------------------------------------------------------------------------------
//
//------------------  lê linha no arquivo aberto f
//
//------------------------------------------------------------------------------------------

bool brwSpiffs::leLinha (File *f, char * linha) {
  int i = 0;
  String str;
  //char ch;

  takeMutex ();
  str = f -> readStringUntil('\r');
  strcpy (linha,str.c_str());
  str = f -> readStringUntil('\n');
  /*
  while(f -> available()){
      ch = f -> read();
      linha [i] = ch;
      if (ch = '\n')
        return (true);
      i++;
    }
    */
  giveMutex ();  
  return(false);  
}


