#include <SPI.h>
#include <WiFi101.h>


#include "/home/adam/realdash_arduino/Arduino/arduino_secrets.h" 
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


void setup() {
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
 
  analogWrite(A1, 0);
}

void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  unsigned long canFrameId1 = 3200;
  unsigned long canFrameId2 = 3203;
  byte buf1[8];
  byte buf2[8];
  
unsigned int rpm =  5252;


  buf1[0] = ((rpm >> 8) & 0xff);
  buf1[1] = (rpm & 0xff);
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
      analogWrite(A1, value);
      if (value == 0) {
        zeroBoostvars();
        boostLevel1 = 1;
      }
      if (value == 50) {
        zeroBoostvars();
        boostLevel2 = 1;
      }
      if (value == 100) {
        zeroBoostvars();
        boostLevel2 = 1;
      }
      if (value == 150) {
        zeroBoostvars();
        boostLevel3 = 1;
      }
      if (value == 205) {
        zeroBoostvars();
        boostLevel4 = 1;
      }
      if (value == 250) {
        zeroBoostvars();
        boostLevel5 = 1;
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

void zeroBoostvars() {
        boostLevel1 = 0;
        boostLevel2 = 0;
        boostLevel3 = 0;
        boostLevel4 = 0;
        boostLevel5 = 0;
        boostLevel6 = 0;
}
