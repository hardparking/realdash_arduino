#include <SPI.h>
#include <WiFi101.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>

Adafruit_SSD1306 display = Adafruit_SSD1306();

#if defined(ESP8266)
  #define BUTTON_A 0
  #define BUTTON_B 16
  #define BUTTON_C 2
  #define LED      0
#elif defined(ESP32)
  #define BUTTON_A 15
  #define BUTTON_B 32
  #define BUTTON_C 14
  #define LED      13
#elif defined(ARDUINO_STM32F2_FEATHER)
  #define BUTTON_A PA15
  #define BUTTON_B PC7
  #define BUTTON_C PC5
  #define LED PB5
#elif defined(TEENSYDUINO)
  #define BUTTON_A 4
  #define BUTTON_B 3
  #define BUTTON_C 8
  #define LED 13
#elif defined(ARDUINO_FEATHER52)
  #define BUTTON_A 31
  #define BUTTON_B 30
  #define BUTTON_C 27
  #define LED 17
#else // 32u4, M0, and 328p
  #define BUTTON_A 9
  #define BUTTON_B 6
  #define BUTTON_C 5
  #define LED      13
#endif

#include "arduino_secrets.h" 
//please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP
int status = WL_IDLE_STATUS;

WiFiServer server(8000);

const unsigned long serialBlockTag = 0x11223344;

// Arduino digital and analog pins
unsigned int digitalPins = 0;
int analogPins[7] = {0};
int count = 0;

int boostLevel1 = 1;
int boostLevel2 = 0;
int boostLevel3 = 0;
int boostLevel4 = 0;
int boostLevel5 = 0;
int boostLevel6 = 0;
int boostLevel = 1;


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  WiFi.setPins(8,7,4,2);
  Serial.begin(115200);
  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);
  
  status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    while(true);
  } 
  else {
    server.begin();
  }

  IPAddress ip = WiFi.localIP();
  ip = WiFi.localIP();
  
  analogWrite(A1, 0);

  display.setCursor(0,0);
  display.print("SSID:");
  display.print(SECRET_SSID);
  display.print("\n");
  display.print("IP:");
  display.println(ip);
  display.println("Port: 8000");
  display.println("Boost Level: 0");
  display.setCursor(0,0);
  display.display(); // actually display all of the above
  delay(5000);
  display.clearDisplay();
  display.display();
  
  
}

void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  unsigned long canFrameId1 = 3200;
  unsigned long canFrameId2 = 3203;
  byte buf1[8];
  byte buf2[8];
  buttons(boostLevel);
  boostSub(boostLevel);
  drawDisplay(boostLevel);
  delay(90); 
  unsigned int amb =  0;
  

  buf1[0] = ((amb >> 8) & 0xff);
  buf1[1] = (amb & 0xff);
  buf1[2] = ((boostLevel1 >> 8) & 0xff);
  buf1[3] = (boostLevel1 & 0xff);
  buf1[4] = ((boostLevel2 >> 8) & 0xff);
  buf1[5] = (boostLevel2 & 0xff);
  buf1[6] = ((boostLevel3 >> 8) & 0xff);
  buf1[7] = (boostLevel3 & 0xff);

  buf2[0] = ((boostLevel4 >> 8) & 0xff);
  buf2[1] = (boostLevel4 & 0xff);
  buf2[2] = ((boostLevel5 >> 8) & 0xff);
  buf2[3] = (boostLevel5 & 0xff);
  buf2[4] = ((boostLevel6 >> 8) & 0xff);
  buf2[5] = (boostLevel6 & 0xff);

  
  if (client == true) {
    SendCANFrameToSerial(canFrameId1, buf1);
    SendCANFrameToSerial(canFrameId2, buf2);
    ReadIncomingSerialData();
  }  
}

void SendCANFrameToSerial(unsigned long canFrameId, const byte* frameData)
{
  // the 4 byte identifier at the beginning of each CAN frame
  // this is required for RealDash to 'catch-up' on ongoing stream of CAN frames
  server.write((const byte*)&serialBlockTag, 4);
  //Serial.write((const byte*)&serialBlockTag, 4);
  // the CAN frame id number
  server.write((const byte*)&canFrameId, 4);
  //Serial.write((const byte*)&canFrameId, 4);
  // CAN frame payload
  server.write(frameData, 8);
  //Serial.write(frameData, 8);
}

void ReadIncomingSerialData()
{
  WiFiClient client = server.available();
  while (client.available() > 13)
  {
    // we have at least 14 bytes available (one RealDash 'SET VALUE' frame)

    // read entire set value frame to byte buffer
    byte frame[14] = { 0 };
    client.read(frame, 14);

    // first four bytes contain set value frame separator bytes
    unsigned long blockTag = 0;
    memcpy(&blockTag, frame, 4);

    // next four bytes contain set value CAN frame id
    unsigned long canFrameId = 0;
    memcpy(&canFrameId, frame + 4, 4);

    // next byte is index of the value in this CAN frame
    byte valueIndex = 0;
    memcpy(&valueIndex, frame + 8, 1);

    // next 4 bytes are the value payload
    unsigned long value = 0;
    memcpy(&value, frame + 9, 4);

    // last byte is check byte calculated as sum of previous 13 bytes (ignore overflow)
    byte checkByte = 0;
    for (int i=0; i<13; i++)
    {
      checkByte += frame[i];
    }
    
    if (blockTag == serialBlockTag && frame[13] == checkByte)
    {
      // this is a valid set value-frame:
      // it begins with RealDash CAN frame separator and last check byte calculation match
      HandleIncomingSetValueFrame(canFrameId, valueIndex, value);
    }
  }
}

void HandleIncomingSetValueFrame(unsigned long canFrameId, byte valueIndex, unsigned long value)
{
  if (canFrameId == 3201)
  {
    if (valueIndex < 13)
    {
      // values 0-12 in frame 3201 are the digital pins
      digitalWrite(valueIndex + 1, (value) ? HIGH : LOW);
    }
    else if (valueIndex < 16)
    {
      // values 13-15 in frame 3201 are analog pins 0-2
      //analogWrite(valueIndex - 13, (unsigned int)value);
      
      if (value == 1) {
        boostSub(1);
      }
      if (value == 2) {
        boostSub(2);
      }
      if (value == 3) {
        boostSub(3);
      }
      if (value == 4) {
        boostSub(4);
      }
      if (value == 5) {
        boostSub(5);
      }
      if (value == 6) {
        boostSub(6);
      }
    }
  }
  else if (canFrameId == 3202)
  {
    if (valueIndex < 4)
    {
      // values 0-3 in frame 3202 are the analog pins 3-6
        analogWrite(3 + valueIndex, (unsigned int)value);     
    }
  }
}

void buttons(int x) {
  if(! digitalRead(BUTTON_A)){
    if (x <= 4) {
      boostLevel++;
      //boostSub(boostLevel);
    }
  }
  
  if (! digitalRead(BUTTON_C)){
    if (x >= 2) {
      boostLevel--;
    }
  }
  if (! digitalRead(BUTTON_B)){
    display.print("B");
  }
}
void drawDisplay(int x) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(3);
  display.print("Boost:");
  display.println(x);
  display.display();
}

void zeroBoostvars() {
        boostLevel1 = 0;
        boostLevel2 = 0;
        boostLevel3 = 0;
        boostLevel4 = 0;
        boostLevel5 = 0;
        boostLevel6 = 0;
}

void boostSub( int x ) {
  if (x == 1) {
    zeroBoostvars();
    boostLevel1 = 1;
    boostLevel = 1;
    analogWrite(A1, 0);
  }
  if (x == 2) {
    zeroBoostvars();
    boostLevel2 = 1;
    boostLevel = 2;
    analogWrite(A1, 50);
  }
  if (x == 3) {
    zeroBoostvars();
    boostLevel3 = 1;
    boostLevel = 3;
    analogWrite(A1, 100);
  }
  if (x == 4) {
    zeroBoostvars();
    boostLevel4 = 1;
    boostLevel = 4;
    analogWrite(A1, 150);
  }
  if (x == 4) {
    zeroBoostvars();
    boostLevel4 = 1;
    boostLevel = 4;
    analogWrite(A1, 150);
  }
  if (x == 5) {
    zeroBoostvars();
    boostLevel5 = 1;
    boostLevel = 5;
    analogWrite(A1, 200);
  }
  if (x == 6) {
    zeroBoostvars();
    boostLevel6 = 1;
    boostLevel = 6;
    analogWrite(A1, 250);
  }
}

