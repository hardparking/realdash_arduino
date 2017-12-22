
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
} channels[256] = {
  { NULL, NULL, 0 },
  { "RPM", "RPM", 1 },
  { "MAP", "kPa", 1 },
  { "TPS", "%", 1 },
  { "IAT", "C", 1 },
  { "Batt", "V", 37 },
  { "IgnAngle", "deg", 2 },
  { "pulseWidth", "ms", 62 },
  { "scondarypulseWidth", "ms", 62 },
  { "Egt1", "C", 1 },
  { "Egt2", "C", 1 },
  { "knockLevel", "V", 51 },
  { "dwellTime", "ms", 20 },
  { "wboAFR", "AFR", 10 },
  { "gear", "", 1 },
  { "Baro", "kPa", 1 },
  { "analogIn1", "V", 51 },
  { "analogIn2", "V", 51 },
  { "analogIn3", "V", 51 },
  { "analogIn4", "V", 51 },
  { "injDC", "%", 2 },
  { "emuTemp", "C", 1 },
  { "oilPressure", "Bar", 16 },
  { "oilTemperature", "C", 1 },
  { "fuelPressure", "Bar", 32 },
  { "CLT", "C", 1 },
  { "flexFuelEthanolContent", "%", 2 },
  { "ffTemp", "C", 1 },
  { "wboLambda", "λ", 128 },
  { "vssSpeed", "km/h", 4 },
  { "deltaFPR", "kPa", 1 },
  { "fuelLevel", "%", 1 },
  { "tablesSet", "", 1 },
  { "lambdaTarget", "λ", 100 },
  { "afrTarget", "AFR", 10 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  { NULL, NULL, 0 },
  {"cel", "", 1}

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


void loop() {
      size_t readlen;
      if (Serial1.available() >= 5) {
        while ((readlen = Serial1.readBytes((char *)&frame, sizeof(frame))) != 0) {
          for (;;) {
            if (frame.magic != 0xa3) {
              memmove(&frame, ((uint8_t *)&frame) + 1, sizeof(emu_frame) - 1);
              frame.checksum = (uint8_t)Serial1.read();
            } else {
              break;
            }
          }
          if (frame.channel == 254) {
            break;
          }
            Serial.print("Channel: ");
            Serial.print(channels[frame.channel].name);
            Serial.print(": ");
            Serial.print((float)be16toh(frame.value) / channels[frame.channel].divisor);
            Serial.print(channels[frame.channel].unit);
            Serial.print("\n");          
            delay(1);
        }
      }
}
