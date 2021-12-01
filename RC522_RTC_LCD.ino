#include "Arduino.h"
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//RC522-SPI
//SDA-GPIO5
//MISO-GPIO19
//MOSI-GPIO23
//SCK-GPIO18
//RST-EN-Pin9

//LCD-I2C
//SDA-GPIO21
//SCL GPIO22
//Slave address-0x27

//DS 3231
//SDA-GPIO21
//SCL-GPIO22
//Slave address-

#define SS_PIN 5
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
LiquidCrystal_I2C lcd (0x27,16,2);



// Inicializalão de um array para guardar os bytes do ID serie da tag
byte nuidPICC[4];
//unsigned long combined = 0;
unsigned long int code=0;
unsigned long int code1=0;
unsigned long int code2=0;
unsigned long int code3=0;
unsigned long int code4=0;
void setup() {


  Wire.begin();
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  lcd.init(21,22);
  lcd.backlight();


  lcd.setCursor(0,1);
  lcd.print("Sem leitura");

 }

void loop() {

  // Deteta a presença de tags no leitor
	  if ( ! rfid.PICC_IsNewCardPresent())
	    return;

	  // Repeat anti collision loop
	  //Verifica se existe colisão entre leituras, entra num loop de verificação de anti colosão
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Deteta o tipo de tag que é lida
  //Caso não seja destes tipos não é da marca MIFARE
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  //Caso os bytes registados da tag anterior não sejam iguais aos que acabaram de ser lidos
  //Foi detetada uma nova tag

  //if (rfid.uid.uidByte[0] != nuidPICC[0] ||
    //rfid.uid.uidByte[1] != nuidPICC[1] ||
    //rfid.uid.uidByte[2] != nuidPICC[2] ||
   // rfid.uid.uidByte[3] != nuidPICC[3] ) {
   // Serial.println(F("A new card has been detected."));




// Conversão dos bytes lidos para um array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];

    }

    //code = (rfid.uid.uidByte[0])&& (rfid.uid.uidByte[1] << 8) && (rfid.uid.uidByte[2] << 16) && (rfid.uid.uidByte[3]<< 24);
   //code1 = ( nuidPICC[0])+( nuidPICC[1] << 8) + ( nuidPICC[2] << 16) + (nuidPICC[3]<< 24);
   //code2 = (nuidPICC[0]<< 24)+(nuidPICC[1] << 16) + (nuidPICC[2] <<8) + (nuidPICC[3]);
  // code3 = ( nuidPICC[3])+ ( nuidPICC[2] << 8) + ( nuidPICC[1] << 16) + (nuidPICC[0]<< 24);
   //code4 = (nuidPICC[3]<< 24)+(nuidPICC[2] << 16) + (nuidPICC[1] <<8) +(nuidPICC[0]);


    	lcd.clear();
    	lcd.setCursor(0,0);
    	lcd.print("ID da ultima tag");
    	lcd.setCursor(0,1);
    	lcd.println(nuidPICC[3]);
    	lcd.setCursor(3,1);
    	lcd.println(nuidPICC[2]);
    	lcd.setCursor(6,1);
    	lcd.println(nuidPICC[1]);
    	lcd.setCursor(9,1);
    	lcd.println(nuidPICC[0]);



    	Serial.println(F("The Tag Serial ID is:"));
    //Serial.print(F("In hex: "));
    //printHex(rfid.uid.uidByte, rfid.uid.size); // @suppress("Invalid arguments")
    Serial.println();
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    printDec2(rfid.uid.uidByte); // @suppress("Invalid arguments")
    Serial.println();
  //}
  //else Serial.println(F("Card read previously."));

  Serial.println();
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void printHex(byte *buffer, byte bufferSize) {


	for (byte i = 0; i < bufferSize; i++) {

		Serial.print(buffer[i] < 0x10 ? " 0" : " ");
		Serial.print(buffer[i], HEX);
	}
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 * Converte formato código ASCII para decimal
 *
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
	  Serial.print(buffer[i] < 0x10 ? " 0" : " ");
		Serial.print(buffer[i]);

	  //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    //Serial.print(buffer[i], DEC);
  }

  	  code1 = ( buffer[0])+( buffer[1]<< 8) + ( buffer[2] << 16) + (buffer[3]<< 24);
     code2 = (buffer[0]<< 24)+(buffer[1] << 16) + (buffer[2] <<8) + (buffer[3]);
     Serial.println("code1");
     Serial.println(code1>>24);
     Serial.println(code1>>16 & 0x00FF);
     Serial.println(code1>>8 & 0x0000FF);
     Serial.println(code1& 0x000000FF);
     Serial.println(code1);

}

void printDec2(byte *buffer) {
	unsigned int e=0;
byte a,b,c,d;

a=buffer[0];
b=buffer[1]<< 8;
c=buffer[2]<< 16;
d=buffer[3]<<24;
	e = a && b && c && d;

	 Serial.print(e);
}
