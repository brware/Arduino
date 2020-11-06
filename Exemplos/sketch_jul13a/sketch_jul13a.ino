
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



void setup() {
  Serial.begin(115200);
  Serial.println ("********************************");
  Serial.println ("- Inicio do Setup");
  Serial.println ("********************************");
  
  // put your setup code here, to run once:
  pinMode(LED_IN, OUTPUT); // pino do LED do NODE MCU
  pinMode(BUZZER, OUTPUT); // pino do BUZZER
  digitalWrite(BUZZER, LOW); 
  pinMode (BTN, INPUT_PULLUP);
  pinMode (BT_BOOT,INPUT_PULLUP);
  
 
 // pinMode(S01, INPUT); // Estado do Alarme
    
  pinMode(S02, INPUT_PULLUP); // Status Portão 01
  pinMode(S03, INPUT_PULLUP); // Status Portão 02
  //pinMode(S04, INPUT_PULLUP); // Status Portão 03
  //pinMode(S05, INPUT_PULLUP); // Status Portão 04
  pinMode(S04, INPUT); // Status Portão 03
  pinMode(S05, INPUT); // Status Portão 04

  
  
  
  pinMode (RL01, OUTPUT); // Alarme/Bollard
  pinMode (RL02, OUTPUT); // Desabilita
  pinMode (RL03, OUTPUT); // Gerador de Neblina
  pinMode (RL04, OUTPUT); // Portão 01
  pinMode (RL05, OUTPUT); // Portão 02
  pinMode (RL06, OUTPUT); // Portão 03
  pinMode (RL07, OUTPUT); // Portão 04

  

}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println ("- Act: Rele 01 (33)");
  digitalWrite (RL01,HIGH);
  vTaskDelay(pdMS_TO_TICKS(2000));
  digitalWrite (RL01,LOW);
  vTaskDelay(pdMS_TO_TICKS(500));  
  
  Serial.println ("- Act: Rele 02 (4)");
  digitalWrite (RL02,HIGH);
  vTaskDelay(pdMS_TO_TICKS(2000));
  digitalWrite (RL02,LOW);
  vTaskDelay(pdMS_TO_TICKS(500));

  Serial.println ("- Act: Rele 03 (16)");
  digitalWrite (RL03,HIGH);
  vTaskDelay(pdMS_TO_TICKS(2000));
  digitalWrite (RL03,LOW);
  vTaskDelay(pdMS_TO_TICKS(500));
  
  Serial.println ("- Act: Rele 04 (12)");
  digitalWrite (RL04,HIGH);
  vTaskDelay(pdMS_TO_TICKS(2000));
  digitalWrite (RL04,LOW);
  vTaskDelay(pdMS_TO_TICKS(500));
  
  Serial.println ("- Act: Rele 05 (27)");
  digitalWrite (RL05,HIGH);
  vTaskDelay(pdMS_TO_TICKS(2000));
  digitalWrite (RL05,LOW);
  vTaskDelay(pdMS_TO_TICKS(500));
  
  Serial.println ("- Act: Rele 06 (26)");
  digitalWrite (RL06,HIGH);
  vTaskDelay(pdMS_TO_TICKS(2000));
  digitalWrite (RL06,LOW);
  vTaskDelay(pdMS_TO_TICKS(500));

  Serial.println ("- Act: Rele 07 (26)");
  digitalWrite (RL07,HIGH);
  vTaskDelay(pdMS_TO_TICKS(2000));
  digitalWrite (RL07,LOW);
  vTaskDelay(pdMS_TO_TICKS(500));

  vTaskDelay(pdMS_TO_TICKS(5000));

  //Serial.print ("Entrada Alarme: ");
  //Serial.println (digitalRead (S01));
  vTaskDelay(pdMS_TO_TICKS(2000));

  Serial.print ("Entrada Porta 01: ");
  Serial.println (digitalRead (S02));
  vTaskDelay(pdMS_TO_TICKS(2000));

  Serial.print ("Entrada Porta 02: ");
  Serial.println (digitalRead (S03));
  vTaskDelay(pdMS_TO_TICKS(2000));

  Serial.print ("Entrada Porta 03: ");
  Serial.println (digitalRead (S04));
  vTaskDelay(pdMS_TO_TICKS(2000));

  Serial.print ("Entrada Porta 04: ");
  Serial.println (digitalRead (S05));
  vTaskDelay(pdMS_TO_TICKS(2000));

  
}
