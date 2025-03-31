#include "dht11.h"

int dht11::Update(){
   
  // Komunikation Starten
    pinMode(pin, OUTPUT);     // Setze PIN als output
    digitalWrite(pin, LOW);
    delay(18);                // MCU Pulls low to send Start 18us
    digitalWrite(pin, HIGH);
    delayMicroseconds(40);    // MCU Pulls High and waits for response 20-40us
    pinMode(pin, INPUT);      // Setze PIN als input

  // Antwort Start mit LOW
    unsigned long Timeout=90; // DHT responds & pulls LOW 80us
    unsigned long LastTime=micros();
    while(digitalRead(pin) == LOW){
      if (micros() - LastTime > Timeout) return DHT11_ERROR_TIMEOUT;
    }
    
  // Antwort Start gefolgt von HIGH
    Timeout=90;               // DHT Pulls HIGH to inidicate 'get ready' 80us
    LastTime=micros();
    while(digitalRead(pin) == HIGH){
      if (micros() - LastTime > Timeout) return DHT11_ERROR_TIMEOUT;
    }

  // Antwort Auslesen
    int PINStatus;
    bool bits[40]; // 5 byte = 40 bit
    for (int bitNo=0; bitNo<40; bitNo++){
      // Start time von bit
      LastTime=micros();

      // Jedes Signal startet mit 50us LOW, warten bis high
      PINStatus=LOW;
      while(PINStatus==LOW){ 
        if (micros() - LastTime > 60) return DHT11_ERROR_TIMEOUT; // 50us LOW time (+Reserve) = 60us
        PINStatus=digitalRead(pin);
      }
	  
      // Signal steht an, warten bis Low / für das bit = false -> 26-28us und für High -> 70us (+50us Low signal)
      while(PINStatus==HIGH){ 
        if (micros() - LastTime > 130) return DHT11_ERROR_TIMEOUT; // 50us + 70us = 120us (+Reserve) = 130us
        PINStatus=digitalRead(pin);
      }
	    
      // Bit Speichern
      bits[bitNo]=micros() - LastTime > 85 ? true : false; // false = 50us + (26-28us) = 78us / true = 50us + 70us = 120us -> wenn time > 78 +reserve = 85us dann true

    }

    // Bits in Bytes umwandeln
    uint8_t bytes[5] = {0};
    for (int i = 0; i < 40; i++) {
        bytes[i / 8] |= (bits[i] << (7 - (i % 8)));  // Schiebe Bit auf die entsprechende Position
    }

  /*
  Antwort:
    1st Byte: Relative Humidity Integral Data in % (Integer Part)
    2nd Byte: Relative Humidity Decimal Data in % (Fractional Part) – Zero for DHT11
    3rd Byte: Temperature Integral in Degree Celsius (Integer Part)
    4th Byte: Temperature in Decimal Data in % (Fractional Part) – Zero for DHT11
    5th Byte: Checksum (Last 8 bits of {1st Byte + 2nd Byte + 3rd Byte+ 4th Byte})
  */

  // Checksumme Prüfen
    uint8_t sum = bytes[0] + bytes[1] + bytes[2] + bytes[3];
    if (bytes[4] != sum) return DHT11_ERROR_CHECKSUM;

  // Daten Speichern
    humidity    = bytes[0]*100+bytes[1];
    temperature = bytes[2]*100+bytes[3];

  // Offset
    humidity    += Offset_Humidity     ;
    temperature += Offset_Temperature  ;
    
  // Fertig
    return DHT11_OK;
}