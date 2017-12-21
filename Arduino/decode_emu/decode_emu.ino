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

const char *channels[256] = {
    "RPM",
    "MAP"
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
          Serial.print("Channel: ");
          Serial.println(frame.channel);
          Serial.print("Magic: ");
          Serial.println(frame.magic, HEX);
          Serial.print("Value: ");
          Serial.println(frame.value);
          Serial.print("Checksum: ");
          Serial.println(frame.checksum);
        }
      }
}
