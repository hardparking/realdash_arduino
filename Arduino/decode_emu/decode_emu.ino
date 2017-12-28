
#       define be16toh(s) \
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

#include "LiquidCrystal.h"
LiquidCrystal lcd(8,9,4,5,6,7);

struct {
  const char *name, *unit;
  float divisor;
  int gaugeMin, gaugeMax;
} channels[256] = {
        { NULL, NULL, 0, 0, 0},
        { "RPM", "RPM", 1, 0, 9000 },
        { "MAP", "kPa", 1, 0, 400 },
        { "TPS", "%", 1, 0, 100 },
        { "IAT", "C", 1, -40, 120 },
        { "Batt", "V", 37, 8, 20 },
        { "IgnAngle", "deg", 2, -20, 60 },
        { "scondarypulseWidth", "ms", 62, 0, 25 },
        { "pulseWidth", "ms", 62, 0, 25 },
        { "Egt1", "C", 1, 0, 1100 },
        { "Egt2", "C", 1, 0, 1100 },
        { "knockLevel", "V", 51, 0, 5 },
        { "dwellTime", "ms", 20, 0, 10 },
        { "wboAFR", "AFR", 10, 10, 20 },
        { "gear", "", 1, 0, 6 },
        { "Baro", "kPa", 1, 50, 120 },
        { "analogIn1", "V", 51, 0, 5 },
        { "analogIn2", "V", 51, 0, 5 },
        { "analogIn3", "V", 51, 0, 5 },
        { "analogIn4", "V", 51, 0, 5 },
        { "injDC", "%", 2, 0, 100 },
        { "emuTemp", "C", 1, -40, 120 },
        { "oilPressure", "Bar", 16, 0, 12 },
        { "oilTemperature", "C", 1, 0, 160 },
        { "fuelPressure", "Bar", 32, 0, 7 },
        { "CLT", "C", 1, -40, 220 },
        { "flexFuelEthanolContent", "%", 2, 0, 100 },
        { "ffTemp", "C", 1, -30, 120 },
        { "wboLambda", "λ", 128, 0.7, 1.3 },
        { "vssSpeed", "km/h", 4, 0, 300 },
        { "deltaFPR", "kPa", 1, 100, 500 },
        { "fuelLevel", "%", 1, 0, 100 },
        { "tablesSet", "", 1, 0, 1 },
        { "lambdaTarget", "λ", 100, 0.7, 1.3 },
        { "afrTarget", "AFR", 10, 10, 20 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
        { NULL, NULL, 0, 0, 0 },
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
      lcd.begin(16,2);
      lcd.setCursor(0,0);
      lcd.print("EcuMasters");
      lcd.setCursor(0,1);
      lcd.print("   EMU");
      Serial.begin(19200);
      Serial1.begin(19200);
}

void calculate_checksum(emu_frame frame) {
  return (frame.channel + frame.magic
    + ((frame.value & 0xff00) >> 8)
    +  (frame.value & 0x00ff)) & 0xff;
}

int checksum = 0;

void loop() {
      size_t readlen;
      emu_frame raw;
      if (Serial1.available() >= 5) {
        while ((readlen = Serial1.readBytes((char *)&frame, sizeof(frame))) != 0) {
          uint8_t checksum;
          for (;;) {
            if (frame.magic != 0xa3) {
              memmove(&frame, ((uint8_t *)&frame) + 1, sizeof(emu_frame) - 1);
              frame.checksum = (uint8_t)Serial1.read();
            } else {
              break;
            }
          };  
          checksum = frame.channel + frame.magic + ((frame.value & 0xff00) >> 8) + (frame.value & 0x00ff) % 255;
          if ((float)be16toh(frame.value) / channels[frame.channel].divisor > channels[frame.channel].gaugeMax) {
            continue;
          }
          //if ((float)be16toh(frame.value) / channels[frame.channel].divisor < channels[frame.channel].gaugeMin) {
          //  continue;
          //}
            Serial.print("Channel: ");
            Serial.print(channels[frame.channel].name);
            Serial.print(": ");
            Serial.print((float)be16toh(frame.value) / channels[frame.channel].divisor);
            Serial.print(" ");
            Serial.print(channels[frame.channel].unit);
            Serial.print(" ");
            Serial.print("\n");
            delay(200);
        }
      }
}
