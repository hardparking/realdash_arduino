#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#define BOXSIZE 40
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750

#define be16toh(s) \
  ((uint16_t)(((s & 0xff00) >> 8) | ((s & 0x00ff) << 8)))

#define EMU_FRAME_MAGIC 0xa3
#pragma pack(push)
#pragma pack(1)

typedef struct _emu_frame {
  uint8_t channel,
          magic;
  uint16_t value;
  uint8_t checksum;
} emu_frame;

emu_frame frame;

#pragma pack(pop)

#if defined (__AVR_ATmega32U4__) || defined(ARDUINO_SAMD_FEATHER_M0) || defined (__AVR_ATmega328P__)
#define STMPE_CS 6
#define TFT_CS   9
#define TFT_DC   10
#define SD_CS    5
#endif

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

int checksum = 0;

struct {
  const char *position;
  int emu_channel;
} frontPage[6] = {
  { "1a", 1},
  { "1b", 2},
  { "2a", 3},
  { "2b", 4},
  { "3a", 5},
  { "3b", 6}
};

uint16_t values[256];


void render_rpm() {
  tft.println(be16toh(values[1]));
}

void render_map(uint8_t channel) {
  tft.println(be16toh(values[channel]));
}

void render_tps(uint8_t channel) {
  tft.println(be16toh(values[channel]));
}

void render_iat(uint8_t channel) {
  tft.println(be16toh(values[channel]));
}

void render_bat(uint8_t channel) {
  tft.println(be16toh(values[channel]) / 37);
}

void render_ign(uint8_t channel) {
  tft.println(be16toh(values[channel]) / 2);
}

struct {
  const char *name, *unit;
  float divisor;
  int gaugeMin, gaugeMax;
  void (*render)();
} channels[256] = {
  { NULL, NULL, 0, 0, 0},
  { "RPM", "RPM", 1, 0, 9000, render_rpm  },
  { "MAP", "kPa", 1, 0, 400 },
  { "TPS", "%", 1, 0, 100 },
  { "IAT", "C", 1, -40, 120 },
  { "Battery", "V", 37, 8, 20 },
  { "Ign. Angle", "deg", 2, -20, 60 },
  { "Sec. inj. PW", "ms", 62, 0, 25 },
  { "Inj. PW", "ms", 62, 0, 25 },
  { "EGT #1", "C", 1, 0, 1100 },
  { "EGT #2", "C", 1, 0, 1100 },
  { "Knock Level", "V", 51, 0, 5 },
  { "Dwell Time", "ms", 20, 0, 10 },
  { "AFR", "AFR", 10, 10, 20 },
  { "Gear", "", 1, 0, 6 },
  { "BARO", "kPa", 1, 50, 120 },
  { "Analog #1", "V", 51, 0, 5 },
  { "Analog #2", "V", 51, 0, 5 },
  { "Analog #3", "V", 51, 0, 5 },
  { "Analog #4", "V", 51, 0, 5 },
  { "Inj. DC", "%", 2, 0, 100 },
  { "ECU Temp.", "C", 1, -40, 120 },
  { "Oil press.", "Bar", 16, 0, 12 },
  { "Oil temp.", "C", 1, 0, 160 },
  { "Fuel press.", "Bar", 32, 0, 7 },
  { "CLT", "C", 1, -40, 220 },
  { "FF content", "%", 2, 0, 100 },
  { "FF Temp", "C", 1, -30, 120 },
  { "Lambda", "λ", 128, 0.7, 1.3 },
  { "Speed", "km/h", 4, 0, 300 },
  { "FP delta", "kPa", 1, 100, 500 },
  { "Fuel level", "%", 1, 0, 100 },
  { "Tables set", "", 1, 0, 1 },
  { "Lambda target", "λ", 100, 0.7, 1.3 },
  { "AFR Target", "AFR", 10, 10, 20 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  { NULL, NULL, 0, 0, 0 },
  {"cel", "", 1, 0, 0}
};


void setup() {
  
  memset(&values, 0, sizeof(values));
  
  tft.begin();
  if (!ts.begin()) {
    Serial.println("Couldn't start touchscreen controller");
    while (1);
  }
  Serial.println("Touchscreen started");

  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  
  tft.drawRect(5, 5, 155, 75, ILI9341_WHITE); //1a
  //tft.fillRect(10, 10, 145, 65, ILI9341_RED);
  
  tft.drawRect(165, 5, 155, 75, ILI9341_WHITE); //1b
  //tft.fillRect(170, 10, 145, 65, ILI9341_RED);
  
  tft.drawRect(5, 85, 155, 75, ILI9341_WHITE); //2a
  //tft.fillRect(10, 90, 145, 65, ILI9341_RED);
  
  tft.drawRect(165, 85, 155, 75, ILI9341_WHITE); //2b
  //tft.fillRect(170, 90, 145, 65, ILI9341_RED);

  tft.drawRect(5, 165, 155, 75, ILI9341_WHITE); //3a
  //tft.fillRect(10, 170, 145, 65, ILI9341_RED);
  
  tft.drawRect(165, 165, 155, 75, ILI9341_WHITE); //3b
  //tft.fillRect(170, 170, 145, 65, ILI9341_RED);
  
  
  
  Serial.begin(19200);
  Serial1.begin(19200);
  tft.setCursor(20, 15);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(5);
  tft.setCursor(20, 15);

  
}

void DEBUG() {
  //put testing stuff here
}


int i;
int newchannel;
int count = 1;
int d1a = 1;
void loop() {
  
  TS_Point p = ts.getPoint();
  size_t readlen;
  emu_frame raw;
  /*
  if (Serial1.available() >= 5) {
    while ((readlen = Serial1.readBytes((char *)&frame, sizeof(frame))) != 0) {
      uint8_t checksum;
      for (;;) {
        checksum = frame.channel + frame.magic + ((frame.value & 0xff00) >> 8) + (frame.value & 0x00ff) % 255;
        if (checksum == frame.checksum) {
          break;
        }
        if (frame.magic != 0xa3) {
          memmove(&frame, ((uint8_t *)&frame) + 1, sizeof(emu_frame) - 1);
          frame.checksum = (uint8_t)Serial1.read();
        } else {
          break;
        }
      }
      
      values[frame.channel] = frame.value;
      */
      p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
      p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
      if (ts.touched()) {
        //tft.fillScreen(ILI9341_BLACK);
        TS_Point p = ts.getPoint();
        tft.setCursor(0, 0);
        if (p.x < 1200 && p.y < 1800) {
          Serial.println("1a pushed");
          tft.fillRect(10, 10, 145, 65, ILI9341_RED);
        }
        if (p.x < 1200 && p.y > 2200) {
          Serial.println("1b pushed");
        }
        if (p.x > 1500 && p.x < 2500 && p.y < 1900) {
          Serial.println("2a pushed");
        }
        if (p.x > 1500 && p.x < 2500 && p.y > 2000) {
          Serial.println("2b pushed");
        }
        if (p.x > 2800 && p.y < 1800) {
          Serial.println("3a pushed");
        }
        if (p.x > 2800 && p.y > 1800) {
          Serial.println("3b pushed");
        }
      }

      values[1] = count;
      count++;
      tft.setCursor(140, 8);
      tft.setTextSize(1);
      tft.println(channels[d1a].unit);
      tft.setCursor(10, 62);
      tft.setTextSize(2);
      tft.println(channels[d1a].name);
      tft.setCursor(20, 15);
      tft.setTextSize(5);
      channels[d1a].render();
      //render_rpm();
      /*
      
      //1a
      tft.drawRect(5, 5, 155, 75, ILI9341_WHITE);
      tft.setCursor(140, 8);
      tft.setTextSize(1);
      tft.println(channels[frontPage[0].emu_channel].unit);
      tft.setCursor(10, 62);
      tft.setTextSize(2);
      tft.println(channels[frontPage[0].emu_channel].name);
      tft.setCursor(20, 15);
      tft.setTextSize(5);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.println(values[frontPage[0].emu_channel], 0);
      tft.setTextColor(ILI9341_WHITE);

      Serial.print("Channel: ");
      Serial.print(values[frontPage[0].emu_channel] );
      Serial.print(" ");
      Serial.println(frame.channel);

      //1b
      tft.drawRect(165, 5, 155, 75, ILI9341_WHITE);
      tft.setCursor(300, 8);
      tft.setTextSize(1);
      tft.println(channels[frontPage[1].emu_channel].unit);
      tft.setCursor(170, 62);
      tft.setTextSize(2);
      tft.println(channels[frontPage[1].emu_channel].name);
      tft.setCursor(180, 15);
      tft.setTextSize(5);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.println(values[frontPage[1].emu_channel], 0);


      //2a
      tft.drawRect(5, 85, 155, 75, ILI9341_WHITE);
      tft.setCursor(140, 88);
      tft.setTextSize(1);
      tft.println(channels[frontPage[2].emu_channel].unit);
      tft.setCursor(10, 142);
      tft.setTextSize(2);
      tft.println(channels[frontPage[2].emu_channel].name);
      tft.setCursor(20, 95);
      tft.setTextSize(5);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.println(values[frontPage[2].emu_channel], 0);

      //2b
      tft.drawRect(165, 85, 155, 75, ILI9341_WHITE);
      tft.setCursor(300, 88);
      tft.setTextSize(1);
      tft.println(channels[frontPage[3].emu_channel].unit);
      tft.setCursor(170, 142);
      tft.setTextSize(2);
      tft.println(channels[frontPage[3].emu_channel].name);
      tft.setCursor(180, 95);
      tft.setTextSize(5);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.println(values[frontPage[3].emu_channel], 0);

      //3a
      tft.drawRect(5, 165, 155, 75, ILI9341_WHITE);
      tft.setCursor(140, 168);
      tft.setTextSize(1);
      tft.println(channels[frontPage[4].emu_channel].unit);
      tft.setCursor(10, 222);
      tft.setTextSize(2);
      tft.println(channels[frontPage[4].emu_channel].name);
      tft.setCursor(20, 175);
      tft.setTextSize(5);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.println(values[frontPage[4].emu_channel], 0);

      //3b
      tft.drawRect(165, 165, 155, 75, ILI9341_WHITE);
      tft.setCursor(300, 168);
      tft.setTextSize(1);
      tft.println(channels[frontPage[5].emu_channel].unit);
      tft.setCursor(170, 222);
      tft.setTextSize(2);
      tft.println(channels[frontPage[5].emu_channel].name);
      tft.setCursor(180, 175);
      tft.setTextSize(5);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.println(values[frontPage[5].emu_channel], 0);
      */
    //}
  //}
}
