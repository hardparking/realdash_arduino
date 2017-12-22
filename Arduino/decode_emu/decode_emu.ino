
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
const char *units[256] = {
  "bunk",
  "RPM",
  "kPa",
  "%",
  "C",
  "V",
  "deg",
  "ms",
  "C",
  "C",
  "V",
  "ms",
  "AFR",
  "gear",
  "kPa",
  "V",
  "V",
  "V",
  "V",
  "%",
  "C",
  "Bar",
  "C",
  "Bar",
  "C",
  "",
  "C",
  "λ",
  "km/h",
  "kPa",
  "%",
  "Table Set",
  "λ",
  "AFR", 
};

const char *channels[256] = {
    "bunk",
    "RPM",
    "MAP",
    "TPS",
    "IAT",
    "Batt",
    "IgnAngle",
    "pulseWidth",
    "Egt1",
    "Egt2",
    "knockLevel",
    "dwellTime",
    "wboAFR",
    "gear",
    "Baro",
    "AnalogIn1",
    "AnalogIn2",
    "AnalogIn3",
    "AnalogIn4",
    "injDC",
    "emuTemp",
    "oilPressure",
    "oilTemperature",
    "fuelPressure",
    "CLT",
    "flexFuelEthanolContent",
    "ffTemp",
    "wboLambda",
    "vssSpeed",
    "deltaFPR",
    "cel",
    "fuelLevel",
    "fuelLevel",
    "lambdaTarget",
    ""
};

void setup() {
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
          if (channels[frame.channel] == "oilPressure") {
          Serial.print(channels[frame.channel]);
          Serial.print(": ");
          Serial.print(be16toh(frame.value), DEC);
          Serial.print(" ");
          Serial.print(units[frame.channel]);
          Serial.print("\n");
          delay(1);
          }
        }
      }
}
