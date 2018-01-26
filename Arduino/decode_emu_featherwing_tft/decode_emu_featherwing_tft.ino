#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include "wiring_private.h"
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750
#define EMU_FRAME_MAGIC 0xa3
#define interruptPin 16
#if defined (__AVR_ATmega32U4__) || defined(ARDUINO_SAMD_FEATHER_M0) || defined (__AVR_ATmega328P__)
#define STMPE_CS 6
#define TFT_CS   9
#define TFT_DC   10
#define SD_CS    5
#endif
#define be16toh(s) \
  ((uint16_t)(((s & 0xff00) >> 8) | ((s & 0x00ff) << 8)))

#pragma pack(push)
#pragma pack(1)


typedef struct _emu_frame {
  union {
    struct {
      uint8_t channel,
              magic;
      uint16_t value;
      uint8_t checksum;
    };
    uint8_t bytes[5];
  };
} emu_frame;

emu_frame frame;

#pragma pack(pop)

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

int checksum = 0;
int page = 1;
uint16_t values[35];

uint16_t rpm;
void render_rpm() {
  if (rpm != values[1] && be16toh(rpm) <= 9000 && be16toh(rpm) >= 0) {
    tft.println(be16toh(values[1]));
    rpm = values[1];
  }
}

uint16_t mapv;
void render_map() {
  if (mapv != values[2] && be16toh(mapv) <= 400 && be16toh(mapv) >= 0) {
    tft.println(be16toh(values[2]));
    mapv = values[2];
  }
}

uint16_t tps;
void render_tps() {
  if (tps != values[3] && be16toh(tps) <= 100 && be16toh(tps) >= 0) {
    tft.println(be16toh(values[3]));
    tps = values[3];
  }
}

uint16_t iat;
void render_iat() {
  if (iat != values[4] && be16toh(iat) <= 120 && be16toh(iat) >= -140 ) {
    tft.println(be16toh(values[4l]));
    iat = values[4];
  }
}

uint16_t bat;
void render_bat() {
  if (bat != values[5] && be16toh(bat) <= 20 && be16toh(bat) >= 8) {
    tft.println(be16toh(values[5]) / 37);
    bat = values[5];
  }
}

uint16_t ign;
void render_ign() {
  if (ign != values[6] && be16toh(ign) <= 60 && be16toh(ign) >= -20) {
    tft.println(be16toh(values[6]) / 2);
    ign = values[6];
  }
}

uint16_t secinjpw;
void render_secinjpw() {
  if (secinjpw != values[7] && be16toh(secinjpw) <= 25 && be16toh(secinjpw) >= 0 ) {
    tft.println(be16toh(values[7]) / 62);
    secinjpw = values[7];
  }
}

uint16_t injpw;
void render_injpw() {
  if (injpw != values[8] && be16toh(injpw) <= 25 && be16toh(injpw) >= 0 ) {
    tft.println(be16toh(values[8]) / 62);
    injpw = values[8];
  }
}

uint16_t egt1;
void render_egt1() {
  if (egt1 != values[9] && be16toh(egt1) <= 1100 && be16toh(egt1) >= 0) {
    tft.println(be16toh(values[9]));
    egt1 = values[9];
  }
}

uint16_t egt2;
void render_egt2() {
  if (egt2 != values[10] && be16toh(egt2) <= 1100 && be16toh(egt2) >= 0) {
    tft.println(be16toh(values[10]));
    egt2 = values[10];
  }
}

uint16_t knockv;
void render_knockv() {
  if (knockv != values[11] && be16toh(knockv) <= 5 && be16toh(knockv) >= 0) {
    tft.println(be16toh(values[11]) / 51);
    knockv = values[11];
  }
}

uint16_t dwell;
void render_dwell() {
  if (dwell != values[12] && be16toh(dwell) <= 10 && be16toh(dwell) >= 0 ) {
    tft.println(be16toh(values[12]) / 20);
    dwell = values[12];
  }
}

uint16_t afr;
void render_afr() {
  if (afr != values[13] && be16toh(afr) <= 20 && be16toh(afr) >= 0) {
    tft.println(be16toh(values[13]) / 10);
    afr = values[13];
  }
}

uint16_t gear;
void render_gear() {
  if (gear != values[14] && be16toh(gear) <= 6 && be16toh(gear) >= 0) {
    tft.println(be16toh(values[14]));
    gear = values[14];
  }
}

uint16_t baro;
void render_baro() {
  if (baro != values[15] && be16toh(baro) <= 120 && be16toh(baro) >= 50) {
    tft.println(be16toh(values[15]));
    baro = values[15];
  }
}

uint16_t analog1;
void render_analog1() {
  if (analog1 != values[16] && be16toh(analog1) <= 5 && be16toh(analog1) >= 0) {
    tft.println(be16toh(values[16]) / 51);
    analog1 = values[16];
  }
}

uint16_t analog2;
void render_analog2() {
  if (analog2 != values[17] && be16toh(analog2) <= 5 && be16toh(analog2) >= 0) {
    tft.println(be16toh(values[17]) / 51);
    analog2 = values[17];
  }
}

uint16_t analog3;
void render_analog3() {
  if (analog3 != values[18] && be16toh(analog3) <= 5 && be16toh(analog3) >= 0) {
    tft.println(be16toh(values[18]) / 51);
    analog3 = values[18];
  }
}

uint16_t analog4;
void render_analog4() {
  if (analog4 != values[19] && be16toh(analog4) <= 5 && be16toh(analog4) >= 0) {
    tft.println(be16toh(values[19]) / 51);
    analog3 = values[19];
  }
}

uint16_t injdc;
void render_injdc() {
  if (injdc != values[20] && be16toh(injdc) <= 100 && be16toh(injdc) >= 0) {
    tft.println(be16toh(values[20]) / 2);
    injdc = values[20];
  }
}

uint16_t ecutmp;
void render_ecutmp() {
  if (ecutmp != values[21] && be16toh(ecutmp) <= 120 && be16toh(ecutmp) >= -40) {
    tft.println(be16toh(values[21]));
    ecutmp = values[21];
  }
}

uint16_t oilps;
void render_oilps() {
  if (oilps != values[22] && be16toh(oilps) <= 16 && be16toh(oilps) >= 0) {
    tft.println(be16toh(values[22]) / 16);
    oilps = values[22];
  }
}

uint16_t oiltmp;
void render_oiltmp() {
  if (oiltmp != values[23] && be16toh(oiltmp) <= 160 && be16toh(oiltmp) >= 0) {
    tft.println(be16toh(values[23]));
    oiltmp = values[23];
  }
}

uint16_t fuelps;
void render_fuelps() {
  if (fuelps != values[24] && be16toh(fuelps) <= 32 && be16toh(fuelps) >= 0) {
    tft.println(be16toh(values[24]));
    fuelps = values[24];
  }
}

uint16_t clt;
void render_clt() {
  if (clt != values[25] && be16toh(clt) <= 220 && be16toh(clt) >= -40) {
    tft.println(be16toh(values[25]) / 32);
    fuelps = values[25];
  }
}

uint16_t ffcnt;
void render_ffcnt() {
  if (ffcnt != values[26] && be16toh(ffcnt) <= 100 && be16toh(ffcnt) >= 0) {
    tft.println(be16toh(values[26]) / 2);
    ffcnt = values[26];
  }
}

uint16_t fftmp;
void render_fftmp() {
  if (fftmp != values[27] && be16toh(fftmp) <= 120 && be16toh(fftmp) >= -30) {
    tft.println(be16toh(values[27]));
    fftmp = values[27];
  }
}

uint16_t lambda;
void render_lambda() {
  if (lambda != values[28] && be16toh(lambda) <= 1.3 && be16toh(lambda) >= 0.7) {
    tft.println(be16toh(values[28]) / 128);
    lambda = values[28];
  }
}

uint16_t speedd;
void render_speed() {
  if (speedd != values[29] && be16toh(speedd) <= 300 && be16toh(speedd) >= 0) {
    tft.println(be16toh(values[29]) / 4);
    speedd = values[29];
  }
}

uint16_t fpdelta;
void render_fpdelta() {
  if (speedd != values[30] && be16toh(fpdelta) <= 500 && be16toh(fpdelta) >= 100) {
    tft.println(be16toh(values[30]) / 4);
    speedd = values[30];
  }
}

uint16_t fuellvl;
void render_fuellvl() {
  if (fuellvl != values[31] && be16toh(fuellvl) <= 100 && be16toh(fuellvl) >= 0) {
    tft.println(be16toh(values[31]));
    fuellvl = values[31];
  }
}

uint16_t table;
void render_table() {
  if (table != values[32] && be16toh(table) <= 1 && be16toh(table) >= 0) {
    tft.println(be16toh(values[32]));
    table = values[32];
  }
}

uint16_t lambdatgt;
void render_lambdatgt() {
  if (lambdatgt != values[33] && be16toh(lambdatgt) <= 1.3 && be16toh(lambdatgt) >= 0.7) {
    tft.println(be16toh(values[33]) / 100);
    table = values[33];
  }
}

uint16_t afrtgt;
void render_afrtgt() {
  if (afrtgt != values[34] && be16toh(afrtgt) <= 20 && be16toh(afrtgt) >= 10) {
    tft.println(be16toh(values[34]) / 10);
    afrtgt = values[34];
  }
}

uint16_t cel;
void render_cel() {
  if (afr != values[255] && be16toh(cel) <= 1 && be16toh(cel) >= 0) {
    tft.println(be16toh(values[255]));
    cel = values[255];
  }
}

struct {
  const char *name, *unit;
  void (*render)();
} channels[256] = {
  { NULL, NULL, NULL},
  { "RPM", "RPM", render_rpm },
  { "MAP", "kPa", render_map },
  { "TPS", "%", render_tps },
  { "IAT", "C", render_iat },
  { "Battery", "V", render_bat },
  { "Ign. Angle", "deg", render_ign },
  { "Sec. inj. PW", "ms", render_secinjpw },
  { "Inj. PW", "ms", render_injpw },
  { "EGT #1", "C", render_egt1 },
  { "EGT #2", "C", render_egt2 },
  { "Knock Level", "V", render_knockv },
  { "Dwell Time", "ms", render_dwell },
  { "AFR", "AFR", render_afr },
  { "Gear", "", render_gear },
  { "BARO", "kPa", render_baro },
  { "Analog #1", "V", render_analog1 },
  { "Analog #2", "V", render_analog2 },
  { "Analog #3", "V", render_analog3 },
  { "Analog #4", "V", render_analog4 },
  { "Inj. DC", "%", render_injdc },
  { "ECU Temp.", "C", render_ecutmp },
  { "Oil press.", "Bar", render_oilps },
  { "Oil temp.", "C", render_oiltmp },
  { "Fuel press.", "Bar", render_fuelps },
  { "CLT", "C", render_clt },
  { "FF content", "%", render_ffcnt },
  { "FF Temp", "C", render_fftmp },
  { "Lambda", "λ", render_lambda },
  { "Speed", "kmh", render_speed },
  { "FP delta", "kPa", render_fpdelta },
  { "Fuel lvll", "%", render_fuellvl },
  { "Table", "", render_table },
  { "Lambda tgt.", "λ", render_lambdatgt },
  { "AFR Target", "AFR", render_afrtgt },
  {"cel", "", render_cel}
};

void render_page() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.setTextSize(4);
  tft.println(channels[page].name);
  tft.setCursor(280, 210);
  tft.setTextSize(2);
  tft.println(channels[page].unit);
  tft.setTextSize(10);
}



void SERCOM0_Handler() {
  Serial1.IrqHandler();
}

void Uart::IrqHandler() {
  if (sercom->availableDataUART()) {
    rxBuffer.store_char(sercom->readDataUART());
  }

  if (sercom->isUARTError()) {
    sercom->acknowledgeUARTError();
    // TODO: if (sercom->isBufferOverflowErrorUART()) ....
    // TODO: if (sercom->isFrameErrorUART()) ....
    // TODO: if (sercom->isParityErrorUART()) ....
    sercom->clearStatusUART();
  }

  if (rxBuffer.isFull()) {
    size_t available;
    Serial.println("Full Buffer!!");
      for (available = Serial1.available(); available--;) {
      memmove(&frame, ((uint8_t *)&frame) + 1, sizeof(frame) - 1);
      //frame.bytes[4] = rxBuffer.read_char();
      frame.bytes[4] = Serial1.read();
      if (frame.magic == 0xa3) {
        uint8_t checksum = frame.channel + frame.magic + ((frame.value & 0xff00) >> 8) + (frame.value & 0x00ff) & 0xff;
        if (frame.checksum == checksum) {
          values[frame.channel] = be16toh(frame.value);
          memset(&frame, 0, sizeof(frame));
        }
      }
    }
  }
}


void setup() {

  memset(&values, 0, sizeof(values));

  tft.begin();
  if (!ts.begin()) {
    while (1);
  }

  tft.setRotation(1);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  render_page();

  Serial.begin(19200);
  Serial1.begin(19200);

}

void loop() {
  TS_Point p = ts.getPoint();
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());


  tft.setCursor(50, 100);
  channels[page].render();

  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    page++;
    if (page >= 35) {
      page = 1;
    }
    render_page();
  }
}
