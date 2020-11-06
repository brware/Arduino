
#include "brwMutex.h"

//------------------------------------------------------------------------------------------
//
//  Construtor da Classe de configuração de rede
//
//-------------------------------------------------------------------------------------------

brwMutex::brwMutex (){

  myMutex = false;

}


//------------------------------------------------------------------------------------------
//
//  Pega o Mutex, setado como true
//
//-------------------------------------------------------------------------------------------

bool brwMutex::takeMutex (int tmp_ms) {
  bool w;

  w = waitMutex (tmp_ms);
  if (w) {
    myMutex = true;
    return (true); 
  } else 
      return (false);
	

} 

//------------------------------------------------------------------------------------------
//
//  Libera  o Mutex, setado como false
//
//-------------------------------------------------------------------------------------------

bool brwMutex::giveMutex () {

	myMutex = false;

}


//------------------------------------------------------------------------------------------
//
//  Aguarda liberação do mutex o Mutex,
//
//-------------------------------------------------------------------------------------------
        
bool brwMutex::waitMutex (int tmp_ms) {
  int i = 0;

  if (tmp_ms < 0)
      tmp_ms = 0;

  if (tmp_ms == 0) {
    while (!myMutex) {
      vTaskDelay(pdMS_TO_TICKS(100)); 
    }
  } else {
    while (!myMutex) {
      if (i < tmp_ms) {
        i++;
        vTaskDelay(pdMS_TO_TICKS(100)); 
      }
      else
        return (false);
    } 
   
  }

  return (true); 

}