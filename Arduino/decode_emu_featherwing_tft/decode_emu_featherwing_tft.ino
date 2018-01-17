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
int d1a = 1;
int d1b = 2;
int d2a = 3;
int d2b = 4;
int d3a = 5;
int d3b = 6;
uint16_t values[256];


uint16_t rpm;
void render_rpm() {
  if (rpm != values[1]) {
    tft.println(be16toh(values[1]));
    rpm = values[1];
  }
}

uint16_t mapv;
void render_map() {
  if (mapv != values[2]) {
    tft.println(be16toh(values[2]));
    mapv = values[2];
  }
}

uint16_t tps;
void render_tps() {
  if (tps != values[3]) {
    tft.println(be16toh(values[3]));
    tps = values[3];
  }
}

uint16_t iat;
void render_iat() {
  if (iat != values[4]) {
    tft.println(be16toh(values[4l]));
    iat = values[4];
  }
}

uint16_t bat;
void render_bat() {
  if (bat != values[5]) {
    tft.println(be16toh(values[5]) / 37);
    bat = values[5];
  }
}

uint16_t ign;
void render_ign() {
  if (ign != values[6]) {
    tft.println(be16toh(values[6]) / 2);
    ign = values[6];
  }
}

uint16_t secinjpw;
void render_secinjpw() {
  if (secinjpw != values[7]) {
    tft.println(be16toh(values[7]) / 62);
    secinjpw = values[7];
  }
}

uint16_t injpw;
void render_injpw() {
  if (injpw != values[8]) {
    tft.println(be16toh(values[8]) / 62);
    injpw = values[8];
  }
}

uint16_t egt1;
void render_egt1() {
  if (egt1 != values[9]) {
    tft.println(be16toh(values[9]));
    egt1 = values[9];
  }
}

uint16_t egt2;
void render_egt2() {
  if (egt2 != values[10]) {
    tft.println(be16toh(values[10]));
    egt2 = values[10];
  }
}

uint16_t knockv;
void render_knockv() {
  if (knockv != values[11]) {
    tft.println(be16toh(values[11]) / 51);
    knockv = values[11];
  }
}

uint16_t dwell;
void render_dwell() {
  if (dwell != values[12]) {
    tft.println(be16toh(values[12]) / 20);
    dwell = values[12];
  }
}

uint16_t afr;
void render_afr() {
  if (afr != values[13]) {
    tft.println(be16toh(values[13]) / 10);
    afr = values[13];
  }
}

uint16_t gear;
void render_gear() {
  if (gear != values[14]) {
    tft.println(be16toh(values[14]));
    gear = values[14];
  }
}

uint16_t baro;
void render_baro() {
  if (baro != values[15]) {
    tft.println(be16toh(values[15]));
    baro = values[15];
  }
}

uint16_t analog1;
void render_analog1() {
  if (analog1 != values[16]) {
    tft.println(be16toh(values[16]) / 51);
    analog1 = values[16];
  }
}

uint16_t analog2;
void render_analog2() {
  if (analog2 != values[17]) {
    tft.println(be16toh(values[17]) / 51);
    analog2 = values[17];
  }
}

uint16_t analog3;
void render_analog3() {
  if (analog3 != values[18]) {
    tft.println(be16toh(values[18]) / 51);
    analog3 = values[18];
  }
}

uint16_t analog4;
void render_analog4() {
  if (analog4 != values[19]) {
    tft.println(be16toh(values[19]) / 51);
    analog3 = values[19];
  }
}

uint16_t injdc;
void render_injdc() {
  if (injdc != values[20]) {
    tft.println(be16toh(values[20]) / 2);
    injdc = values[20];
  }
}

uint16_t ecutmp;
void render_ecutmp() {
  if (ecutmp != values[21]) {
    tft.println(be16toh(values[21]));
    ecutmp = values[21];
  }
}

uint16_t oilps;
void render_oilps() {
  if (oilps != values[22]) {
    tft.println(be16toh(values[22]) / 16);
    oilps = values[22];
  }
}

uint16_t oiltmp;
void render_oiltmp() {
  if (oiltmp != values[23]) {
    tft.println(be16toh(values[23]));
    oiltmp = values[23];
  }
}

uint16_t fuelps;
void render_fuelps() {
  if (fuelps != values[24]) {
    tft.println(be16toh(values[24]));
    fuelps = values[24];
  }
}

uint16_t clt;
void render_clt() {
  if (clt != values[25]) {
    tft.println(be16toh(values[25]) / 32);
    fuelps = values[25];
  }
}

uint16_t ffcnt;
void render_ffcnt() {
  if (ffcnt != values[26]) {
    tft.println(be16toh(values[26]) / 2);
    ffcnt = values[26];
  }
}

uint16_t fftmp;
void render_fftmp() {
  if (fftmp != values[27]) {
    tft.println(be16toh(values[27]));
    fftmp = values[27];
  }
}

uint16_t lambda;
void render_lambda() {
  if (lambda != values[28]) {
    tft.println(be16toh(values[28]) / 128);
    lambda = values[28];
  }
}

uint16_t speedd;
void render_speed() {
  if (speedd != values[29]) {
    tft.println(be16toh(values[29]) / 4);
    speedd = values[29];
  }
}

uint16_t fpdelta;
void render_fpdelta() {
  if (speedd != values[30]) {
    tft.println(be16toh(values[30]) / 4);
    speedd = values[30];
  }
}

uint16_t fuellvl;
void render_fuellvl() {
  if (fuellvl != values[31]) {
    tft.println(be16toh(values[31]));
    fuellvl = values[31];
  }
}

uint16_t table;
void render_table() {
  if (table != values[32]) {
    tft.println(be16toh(values[32]));
    table = values[32];
  }
}

uint16_t lambdatgt;
void render_lambdatgt() {
  if (lambdatgt != values[33]) {
    tft.println(be16toh(values[33]) / 100);
    table = values[33];
  }
}

uint16_t afrtgt;
void render_afrtgt() {
  if (afrtgt != values[34]) {
    tft.println(be16toh(values[34]) / 10);
    afrtgt = values[34];
  }
}

uint16_t cel;
void render_cel() {
  if (afr != values[255]) {
    tft.println(be16toh(values[255]));
    cel = values[255];
  }
}

struct {
  const char *name, *unit;
  float divisor;
  int gaugeMin, gaugeMax;
  void (*render)();
} channels[256] = {
  { NULL, NULL, 0, 0, 0},
  { "RPM", "RPM", 1, 0, 9000, render_rpm  },
  { "MAP", "kPa", 1, 0, 400, render_map },
  { "TPS", "%", 1, 0, 100, render_tps },
  { "IAT", "C", 1, -40, 120, render_iat },
  { "Battery", "V", 37, 8, 20, render_bat },
  { "Ign. Angle", "deg", 2, -20, 60, render_ign },
  { "Sec. inj. PW", "ms", 62, 0, 25, render_secinjpw },
  { "Inj. PW", "ms", 62, 0, 25, render_injpw },
  { "EGT #1", "C", 1, 0, 1100, render_egt1 },
  { "EGT #2", "C", 1, 0, 1100, render_egt2 },
  { "Knock Level", "V", 51, 0, 5, render_knockv },
  { "Dwell Time", "ms", 20, 0, 10, render_dwell },
  { "AFR", "AFR", 10, 10, 20, render_afr },
  { "Gear", "", 1, 0, 6, render_gear },
  { "BARO", "kPa", 1, 50, 120, render_baro },
  { "Analog #1", "V", 51, 0, 5, render_analog1 },
  { "Analog #2", "V", 51, 0, 5, render_analog2 },
  { "Analog #3", "V", 51, 0, 5, render_analog3 },
  { "Analog #4", "V", 51, 0, 5, render_analog4 },
  { "Inj. DC", "%", 2, 0, 100, render_injdc },
  { "ECU Temp.", "C", 1, -40, 120, render_ecutmp },
  { "Oil press.", "Bar", 16, 0, 12, render_oilps },
  { "Oil temp.", "C", 1, 0, 160, render_oiltmp },
  { "Fuel press.", "Bar", 32, 0, 7, render_fuelps },
  { "CLT", "C", 1, -40, 220, render_clt },
  { "FF content", "%", 2, 0, 100, render_ffcnt },
  { "FF Temp", "C", 1, -30, 120, render_fftmp },
  { "Lambda", "λ", 128, 0.7, 1.3, render_lambda },
  { "Speed", "kmh", 4, 0, 300, render_speed },
  { "FP delta", "kPa", 1, 100, 500, render_fpdelta },
  { "Fuel lvll", "%", 1, 0, 100, render_fuellvl },
  { "Table", "", 1, 0, 1, render_table },
  { "Lambda tgt.", "λ", 100, 0.7, 1.3, render_lambdatgt },
  { "AFR Target", "AFR", 10, 10, 20, render_afrtgt },
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
  {"cel", "", 1, 0, 0, render_cel}
};

void render_d1a() {
  tft.fillRect(7, 7, 150, 69, ILI9341_BLACK);
  tft.setCursor(140, 8);
  tft.setTextSize(1);
  tft.println(channels[d1a].unit);
  tft.setCursor(10, 62);
  tft.setTextSize(2);
  tft.println(channels[d1a].name);
  tft.setTextSize(5);
}

void render_d1b() {
  tft.fillRect(167, 7, 150, 69, ILI9341_BLACK);
  tft.setCursor(300, 8);
  tft.setTextSize(1);
  tft.println(channels[d1b].unit);
  tft.setCursor(170, 62);
  tft.setTextSize(2);
  tft.println(channels[d1b].name);
  tft.setTextSize(5);
}

void render_d2a() {
  tft.fillRect(7, 87, 150, 69, ILI9341_BLACK);
  tft.setCursor(140, 88);
  tft.setTextSize(1);
  tft.println(channels[d2a].unit);
  tft.setCursor(10, 142);
  tft.setTextSize(2);
  tft.println(channels[d2a].name);
  tft.setTextSize(5);
}

void render_d2b() {
  tft.fillRect(167, 87, 150, 69, ILI9341_BLACK);
  tft.setCursor(300, 88);
  tft.setTextSize(1);
  tft.println(channels[d2b].unit);
  tft.setCursor(170, 142);
  tft.setTextSize(2);
  tft.println(channels[d2b].name);
  tft.setTextSize(5);
}

void render_d3a() {
  tft.fillRect(7, 167, 150, 69, ILI9341_BLACK);
  tft.setCursor(140, 168);
  tft.setTextSize(1);
  tft.println(channels[d3a].unit);
  tft.setCursor(10, 222);
  tft.setTextSize(2);
  tft.println(channels[d3a].name);
  tft.setTextSize(5);
}

void render_d3b() {
  tft.fillRect(167, 167, 150, 69, ILI9341_BLACK);
  tft.setCursor(300, 168);
  tft.setTextSize(1);
  tft.println(channels[d3b].unit);
  tft.setCursor(170, 222);
  tft.setTextSize(2);
  tft.println(channels[d3b].name);
  tft.setTextSize(5);
}

void setup() {
  Serial.begin(19200);
  Serial1.begin(19200);

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
  tft.drawRect(165, 5, 155, 75, ILI9341_WHITE); //1b
  tft.drawRect(5, 85, 155, 75, ILI9341_WHITE); //2a
  tft.drawRect(165, 85, 155, 75, ILI9341_WHITE); //2b
  tft.drawRect(5, 165, 155, 75, ILI9341_WHITE); //3a
  tft.drawRect(165, 165, 155, 75, ILI9341_WHITE); //3b

  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  render_d1a();
  render_d1b();
  render_d2a();
  render_d2b();
  render_d3a();
  render_d3b();

}

void DEBUG() {
  //put testing stuff here
}


void loop() {
  TS_Point p = ts.getPoint();
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    if (p.x < 1200 && p.y < 1800) {
      delay(200);
      Serial.println("1a pushed");
      d1a++;
      if (d1a >= 35) {
        d1a = 1;
      }
      render_d1a();
    }
    if (p.x < 1200 && p.y > 2200) {
      delay(200);
      Serial.println("1b pushed");
      d1b++;
      if (d1b >= 35) {
        d1b = 1;
      }
      render_d1b();
    }
    if (p.x > 1500 && p.x < 2500 && p.y < 1900) {
      delay(200);
      Serial.println("2a pushed");
      d2a++;
      if (d2a >= 35) {
        d2a = 1;
      }
      render_d2a();
    }
    if (p.x > 1500 && p.x < 2500 && p.y > 2000) {
      delay(200);
      Serial.println("2b pushed");
      d2b++;
      if (d2b >= 35) {
        d2b++;
      }
      render_d2b();
    }
    if (p.x > 2800 && p.y < 1800) {
      delay(200);
      Serial.println("3a pushed");
      d3a++;
      if (d3a >= 35) {
        d3a++;
      }
      render_d3a();
    }
    if (p.x > 2800 && p.y > 1800) {
      delay(200);
      Serial.println("3b pushed");
      d3b++;
      if (d3b >= 35) {
        d3b++;
      }
      render_d3b();
    }
  }

  size_t readlen;
  emu_frame raw;

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

      //1a
      tft.setCursor(20, 15);
      channels[d1a].render();
      //1b
      tft.setCursor(180, 15);
      channels[d1b].render();
      //2a
      tft.setCursor(20, 95);
      channels[d2a].render();
      //2b
      tft.setCursor(180, 95);
      channels[d2b].render();
      //3a
      tft.setCursor(20, 175);
      channels[d3a].render();
      //3b
      tft.setCursor(180, 175);
      channels[d3b].render();

    }
  }
}
