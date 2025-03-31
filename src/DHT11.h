#ifndef __dht11__
#define __dht11__

  #define Version_dht11 0

  #include <Arduino.h>
  
  #define DHT11_Time         23000  // 18ms+40us+80us+80us+40bit*78us=23ms
                                      // 18ms        = send 0
                                      // 40us        = send 1 (Request)
                                      // 80us        = Receive 0
                                      // 80us        = Receive 1 (Start bit)
                                      // 120us (max) = Receive data bit (total 40 bits)

  #define DHT11_OK               1
  #define DHT11_ERROR_CHECKSUM  -1
  #define DHT11_ERROR_TIMEOUT   -2

  class dht11 {
    private:
      int pin               ;
      int humidity          ;
      int temperature       ;
    public:
      int Offset_Humidity   ;                                 // Offset zum korrigieren
      int Offset_Temperature;                                 // Offset zum korrigieren
    
      dht11(int pin, int Offset_Humidity = 0, int Offset_Temperature = 0) : pin(pin),Offset_Humidity(Offset_Humidity),Offset_Temperature(Offset_Temperature){};
      int   Update();                                          // Aktualisieren / werte auslesen
      int   Humidity()     {return         humidity        ;}; // Luftfeuchtigkeit = n/100
      float Humidity_f()   {return ((float)humidity   )/100;}; // Luftfeuchtigkeit float
      int   Temperature()  {return         temperature     ;}; // Temperatur = n/100
      float Temperature_f(){return ((float)temperature)/100;}; // Temperatur float

  };

#endif
