#include <DHT11.h>

const int  Rx_Tx_DHT  = A7;  // DHT11 - PIN

dht11 DHT11(Rx_Tx_DHT);

void setup() {
  // Start Serial
    Serial.begin(9600);
    while(!Serial);
    Serial.println("Restart");
}

void loop() {
  // Read Data
    int DHTStatus = DHT11.Update();
    Serial.println();

  // Output
    if (DHTStatus == DHT11_OK){
      Serial.print("Humidity    (int)  : ");Serial.println(DHT11.Humidity(     ));
      Serial.print("Humidity    (float): ");Serial.print  (DHT11.Humidity_f(   ));Serial.println("%");
      Serial.print("Temperature (int)  : ");Serial.println(DHT11.Temperature(  ));
      Serial.print("Temperature (float): ");Serial.print  (DHT11.Temperature_f());Serial.println("°C");
  // Error CHECKSUM
    } else if (DHTStatus == DHT11_ERROR_CHECKSUM){
      Serial.println("Error: CHECKSUM");
  // Error TIMEOUT
    } else if (DHTStatus == DHT11_ERROR_TIMEOUT){
      Serial.println("Error: TIMEOUT");
  // Error Undef. Status
    } else {
      Serial.println("Error: Undef. Status");
    }

  // Wait 1 Sec. for next update
    delay(1000);
}
