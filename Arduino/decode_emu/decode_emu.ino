
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


struct {
  char *name, *unit;
  float divisor;
} channels[256] = {
  { NULL, NULL, 0 },
  
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
          printf("Channel: %s, %d, %s\n",
            channels[frame.channel].name,
            (float)frame.value / channels[frame.channel].divisor,
            channels[frame.channel].unit);
          delay(1);
        }
      }
}
