
#ifndef brw_two_h
#define brw_two_h

//------------------------------------------------------------------------------------------
//
//  Define de todos os pinos da placa
//
//-------------------------------------------------------------------------------------------



// Misselania
#define BT_BOOT   0
#define BUZZER    14
#define LED_IN    2
#define BTN       4



//Sensores

#define S02       35  // Sensor portão 01 - 
#define S03       34  // Sensor portão 02 - 
#define S04       32  // Sensor portão 03 - 
#define S05       39  // Sensor portão 04 - 

#define S01 	  36 // sensor s01 do 12V


#define RX10       3  // SERIAL  TTL RX
#define TX11       1  // serial TTL  TX







#define RL01      33 //     - Rele 01 - Alarme / Bollard
#define RL02      4 //     - Rele 02 - Habilita
#define RL03      16 //     - Rele 03 - Gerador de neblina
#define RL04      12 //     - Rele 04 - Portão 01
#define RL05      27 // S01 - Rele 05 (expanssão)  
#define RL06      26 // S04 - Rele 06 (expanssão)
#define RL07      25 // S07 - Rele 07 (expanssão)



// SPI
#define SPI_RST   17
#define SPI_SCS   5
#define SPI_CLK   18
#define SPI_NIS0  19
#define SPI_MOSI  23

// I2C
#define I2C_SCA   21
#define I2C_SCL   22

#endif

