
#ifndef brw_one_h
#define brw_one_h

//------------------------------------------------------------------------------------------
//
//  Define de todos os pinos da placa
//
//-------------------------------------------------------------------------------------------



// Misselania
#define BT_BOOT   0
#define BUZZER    15
#define LED_IN    2
#define BTN       14

//Sensores
#define S01       33  // Entrada e saida, pode ser analogico com WiFi
#define S02       35  // PULLUP DE HARDWARE
#define S03       34  // PULLUP DE HARDWARE

#define S04       32  // Entrada e saida, pode ser analogico com WiFi
#define S05       39  // PULLUP DE HARDWARE
#define S06       36  // PULLUP DE HARDWARE

#define S07       4   // Entrada e saida, pode ser analogico somente sem WiFi
#define S08       16  // Entrada e saida, pode ser analogico somente sem WiFi
#define S09       13  // Entrada e saida, pode ser analogico somente sem WiFi

#define RX10       3  // SERIAL  TTL RX
#define TX11       1  // serial TTL  TX

// reles
#define RL01      12   // Falha a inialização se puxado patra alto
#define RL02      27
#define RL03      26
#define RL04      25

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

