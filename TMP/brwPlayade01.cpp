#include "brwPlayade01.h"




//------------------------------------------------------------------------------------------
//
//  Construtor da Classe de configuração de rede
//
//-------------------------------------------------------------------------------------------

brwPlayade01::brwPlayade01 (){

    R01pulso = true;
    R02pulso = false;
    R03pulso = false;
    R04pulso = false;

    R01tempo = 0;
    R02tempo = 1;
    R03tempo = 0;
    R04tempo = 0;

}


void brwPlayade01::iniPlayade01 (brwSpiffs* Spiffs) {

	pSpiffs = Spiffs;

    

    //initDHT (22, 33, 5);

}




//------------------------------------------------------------------------------------------
//
//  --------    Seta Variaveis do App PLAYADES01
//
//------------------------------------------------------------------------------------------

void brwPlayade01::setMinT (char* v) {

    strncpy (minT, v,sizeof(minT));
}

void brwPlayade01::setMaxT (char* v) {

    strncpy (maxT, v,sizeof(maxT));
}

void brwPlayade01::setMinU (char* v) {

    strncpy (minU, v,sizeof(minU));
}

void brwPlayade01::setMaxU (char* v) {

    strncpy (maxU, v,sizeof(maxU));
}



//------------------------------------------------------------------------------------------
//
//  --------    Get Variaveis do App PLAYADES01
//
//------------------------------------------------------------------------------------------

char* brwPlayade01::getMinT () {

    return (minT);
}

char* brwPlayade01::getMaxT () {

    return (maxT);
}

char* brwPlayade01::getMinU () {

    return (minU);
}

char* brwPlayade01::getMaxU () {

    return (maxU);
}



//------------------------------------------------------------------------------------------
//
//  --------    Get Variaveis de rele PLAYADES01
//
//------------------------------------------------------------------------------------------


void brwPlayade01::setR01pulso (bool st) {

    R01pulso = st;
}

void brwPlayade01::setR02pulso (bool st) {

    R02pulso = st;
}

void brwPlayade01::setR03pulso (bool st) {

    R03pulso = st;
}

void brwPlayade01::setR04pulso (bool st) {

    R04pulso = st;
}
            


bool brwPlayade01::getR01pulso () {

    return (R01pulso);
}
            
bool brwPlayade01::getR02pulso () {

    return (R02pulso);
}

bool brwPlayade01::getR03pulso () {

    return (R03pulso);
}

bool brwPlayade01::getR04pulso () {

    return (R04pulso);
}


void brwPlayade01::setR01tempo (int t) {

    R01tempo = t;
}


void brwPlayade01::setR02tempo (int t) {

    R02tempo = t;
}

void brwPlayade01::setR03tempo (int t) {

    R03tempo = t;
}

void brwPlayade01::setR04tempo (int t) {

    R04tempo = t;
}


int brwPlayade01::getR01tempo () {

    return (R01tempo);
}
 
int brwPlayade01::getR02tempo () {

    return (R02tempo);
}

int brwPlayade01::getR03tempo () {

    return (R03tempo);
}


int brwPlayade01::getR04tempo () {

    return (R04tempo);
}



//------------------------------------------------------------------------------------------
//
// ------    Le arquivo de cogiguracao do Playade 01
//------------------------------------------------------------------------------------------

bool brwPlayade01::getPlay01Config () {

  File wFile;
  char st [20];
  bool b;
  


  if(SPIFFS.exists("/playade01.cfg")) { 

    wFile = SPIFFS.open("/playade01.cfg", "r");
    if(!wFile){
        Serial.println ("Não foi possivel abrir o arquivo de configuração: eth.cfg");
        return (false);  
    }
    Serial.println ("Ler o arquivo de configuração: playade01.cfg");
    
    pSpiffs -> leLinha (& wFile, st);
    setMinT (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pSpiffs -> leLinha (& wFile, st);
    setMaxT (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pSpiffs -> leLinha (& wFile, st);
    setMinU (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pSpiffs -> leLinha (& wFile, st);
    setMaxU (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 


    wFile.close ();
    return (true);
  } else { // le default e cria arquivo
      
      setPlay01Defaut ();  
      
      
      wFile = SPIFFS.open("/playade01.cfg", "w");
      if(!wFile){
          Serial.println ("Não foi possivel (abrir) salvar o arquivo de configuração");
          return (false);  
        }
        

        strncpy (st,getMinT(),sizeof(st));
        pSpiffs -> escreveLinha (& wFile, st);
        vTaskDelay(pdMS_TO_TICKS(100));

        strncpy (st,getMaxT(),sizeof(st));
        pSpiffs -> escreveLinha (& wFile, st);
        vTaskDelay(pdMS_TO_TICKS(100));

        strncpy (st,getMinU(),sizeof(st));
        pSpiffs -> escreveLinha (& wFile, st);
        vTaskDelay(pdMS_TO_TICKS(100));

        strncpy (st,getMaxU(),sizeof(st));
        pSpiffs -> escreveLinha (& wFile, st);
        vTaskDelay(pdMS_TO_TICKS(100));
        

      wFile.close ();
      
      return (true);    
      
    }


}


//------------------------------------------------------------------------------------------
//
// ------    Grava arquivo de configuracao do Harware
//
//------------------------------------------------------------------------------------------
void brwPlayade01::setPlay01Defaut () {

    setMinT ("16");
    setMaxT ("26");
    setMinU ("10");
    setMaxU ("60");


}

//------------------------------------------------------------------------------------------
//
// ------    Print arquivo de configuracao do Harware
//
//------------------------------------------------------------------------------------------

void brwPlayade01::printPlay01Cfg () {


    Serial.println  ("Temperatura Min.: " + String (getMinT()));

    Serial.println  ("Temperatura Max.: " + String (getMaxT()));

    Serial.println  ("Umidade Min.....: " + String (getMinU()));

    Serial.println  ("Umidade Max.....: " + String (getMaxU()));


}


//------------------------------------------------------------------------------------------
//
//  --------    Salva configurações de rede em rede.cfg
//
//------------------------------------------------------------------------------------------


bool brwPlayade01::setPlay01Config () {

File wFile;
String str;

    
    wFile = SPIFFS.open("/playade01.cfg", "w");
    if(!wFile){
        Serial.println ("Não foi possivel salvar o arquivo de configuração");
        return (false);  
    }

        
    wFile.println (getMinT());
    wFile.println (getMaxT());
    wFile.println (getMinU());
    wFile.println (getMaxU());

    wFile.close ();
    //Serial.println ("*** Escreveu arquivo /eth.cfg com sucesso");
    
    return (true);

}
