// link between the computer and the SoftSerial Shield
//at 9600 bps 8-N-1
//Computer is connected to Hardware UART
//SoftSerial Shield is connected to the Software UART:D2&D3 
 
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <Esplora.h>
#include <TFT.h>  // Arduino LCD library

/*
#define	ST7735_BLACK   0x0000
#define	ST7735_BLUE    0x001F
#define	ST7735_RED     0xF800
#define	ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0  
#define ST7735_WHITE   0xFFFF
*/

#define RxD 11
#define TxD  3

SoftwareSerial SoftSerial(RxD, TxD);
unsigned char buffer[64]; // buffer array for data receive over serial port
int count=0; // counter for buffer array 
String authorizedID="27 00 0C 92 57 EE ";

void setup() {
  delay(2000);
  SoftSerial.begin(9600); // the SoftSerial baud rate   
  delay(1000);
  EsploraTFT.begin();
  EsploraTFT.background(0, 0, 0);
  
  // Draw splash screen text
  EsploraTFT.setTextColor(ST7735_YELLOW, ST7735_BLACK);
  EsploraTFT.setTextSize(2);
  EsploraTFT.setCursor(18, 12);
  EsploraTFT.print("RFID Tester");
  EsploraTFT.setTextSize(1);
}

int x=0;

void loop() {
  if (SoftSerial.available()) {
  // if data is coming from softwareserial port ==> data is comming from SoftSerial shield
    Esplora.writeRGB(0xFF, 0x99, 0x00);
    delay(100);
    Esplora.writeRGB(0, 0, 0);
    while(SoftSerial.available()) {
      // reading data into char array
      char c=SoftSerial.read();
      buffer[count++]=c;
      // writing data into array
      if(count == 64)break;
    }
    SoftSerial.flush();
    Esplora.writeRGB(0xFF, 0x99, 0x00);
    delay(100);
    Esplora.writeRGB(0, 0, 0);
    delay(1000);
    if(count>13) {
      String s;
      EsploraTFT.setCursor(0, 33);
      EsploraTFT.println("----------");
      for(char i=1;i<13;i+=2) {
        s=s+char(buffer[i]);
        s=s+char(buffer[i+1]);
        s=s+" ";
      }
      EsploraTFT.setCursor(0, 46);
      EsploraTFT.println(s);
      EsploraTFT.setCursor(0, 57);
      EsploraTFT.println("----------");
      x=14;
      EsploraTFT.fill(0, 0, 0);
      EsploraTFT.stroke(0, 0, 0);
      EsploraTFT.rect(0, 68, 160, 128);
      EsploraTFT.setTextColor(ST7735_YELLOW, ST7735_BLACK);
      // if no data transmission ends, write buffer to hardware serial port
      clearBufferArray();
      // call clearBufferArray function to clear the storaged data from the array
      count = 0;
      // set counter of while loop to zero
      EsploraTFT.setCursor(0, 108);
      if(s==authorizedID) {
        EsploraTFT.println("Access granted");
        Esplora.writeRGB(0, 0xFF, 0);
        delay(100);
        Esplora.writeRGB(0, 0, 0);
        delay(100);
        Esplora.writeRGB(0, 0xFF, 0);
        delay(100);
        Esplora.writeRGB(0, 0, 0);
        delay(100);
        Esplora.writeRGB(0, 0xFF, 0);
        delay(100);
        Esplora.writeRGB(0, 0, 0);
        SoftSerial.write("dda");
      } else {
        EsploraTFT.println("Access denied");
        Esplora.writeRGB(0xFF, 0, 0);
        delay(100);
        Esplora.writeRGB(0, 0, 0);
        delay(100);
        Esplora.writeRGB(0xFF, 0, 0);
        delay(100);
        Esplora.writeRGB(0, 0, 0);
        delay(100);
        Esplora.writeRGB(0xFF, 0, 0);
        delay(100);
        Esplora.writeRGB(0, 0, 0);
      }
    }
  }
}

void clearBufferArray() {
// function to clear buffer array
  for (int i=0; i<count;i++) { buffer[i]=NULL;}
  // clear all index of array with command NULL
}

