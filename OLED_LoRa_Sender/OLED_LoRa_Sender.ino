#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  
#include "SSD1306.h" 
#include "images.h"

//include simple dht library
#include <SimpleDHT.h>

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND  868E6

//dht pin on ttgo board
byte pinDHT22 = 25; //or pin 13/25 see https://rntlab.com/question/esp32-heltec-with-dht22/


unsigned int counter = 0;

SSD1306 display(0x3c, 21, 22);
String rssi = "RSSI --";
String packSize = "--";
String packet ;

//prepare for DHT readings
float temperature = 0;
float humidity = 0;
SimpleDHT22 dht22;
 

void setup() {
  pinMode(16,OUTPUT);
  pinMode(2,OUTPUT);
  
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high
  
  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Sender Test");
  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //LoRa.onReceive(cbk);
//  LoRa.receive();
  Serial.println("init ok");
  display.init();
  display.flipScreenVertically();  
  display.setFont(ArialMT_Plain_10);
   
  delay(1500);
}

void loop() {
  TEMPERATURA();
  delay(2000);
}

void TEMPERATURA(){
dht22.read2(pinDHT22, &temperature, &humidity, NULL);
Serial.print("t= ");
Serial.print(temperature);
Serial.print(" ");
Serial.print("h= ");
Serial.print(humidity);
//Serial.print("% ");
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  
  display.drawString(0, 0, "Sending packet: ");
  display.drawString(90, 0, String(counter));
  Serial.println(String(counter));
  display.display();


 
// send packet
 LoRa.beginPacket();
LoRa.print("t= ");
LoRa.print(temperature);
LoRa.print(" ");
LoRa.print("h= ");
LoRa.print(humidity);
//  LoRa.beginPacket();
//  LoRa.print("hello ");
//  LoRa.print(counter);
 LoRa.endPacket();

  counter++;
  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
