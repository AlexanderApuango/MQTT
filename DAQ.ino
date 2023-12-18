#include <Adafruit_MAX31865.h>
#include <dht.h>
dht DHT;
#define DHT11_PIN 7

int Dig_out = LOW;
int Ana_out = 0;
int cont=0;
// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 max = Adafruit_MAX31865(10, 11, 12, 13);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31865 max = Adafruit_MAX31865(10);

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF 430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL 100.0


void setup() {
  Serial.begin(115200);
   Serial2.begin(115200);
  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");

  max.begin(MAX31865_2WIRE);  // set to 2WIRE or 4WIRE as necessary
}


void loop() {
  uint16_t rtd = max.readRTD();
  int chk = DHT.read11(DHT11_PIN);
  Ana_out = analogRead(A0);
  float ratio = rtd;
  ratio /= 32768;
  Serial.print(DHT.temperature);
  Serial.print("/");
  Serial.print(DHT.humidity);
  Serial.print("/");
  Serial.print(max.temperature(RNOMINAL, RREF));
  Serial.print("/");
  Serial.print(Ana_out);
  Serial.print("/");
  Serial.print(cont);
  Serial.print("/");
  Serial.println(1);


  // Check and print any faults
  uint8_t fault = max.readFault();
  if (fault) {
    Serial.print("Fault 0x");
    Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold");
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold");
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias");
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage");
    }
    max.clearFault();
  }
  cont++;
  delay(500);
  Serial2.print(DHT.temperature);
  Serial2.print("-");
  Serial2.print(DHT.humidity);
  Serial2.print("-");
  Serial2.print(max.temperature(RNOMINAL, RREF));
  Serial2.print("-");
  Serial2.print(Ana_out);
  Serial2.print("-");
  Serial2.print(cont);
  Serial2.print("-");
  Serial2.println(1);
}