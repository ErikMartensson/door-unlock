#include <KeyDuino.h>

KeyDuino keyDuino;

uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t uidLength;
uint8_t correctTag[] = { 4, 201, 240, 154, 165, 60, 128 };

void setup(void) {
  Serial.begin(115200);

  keyDuino.begin();

  keyDuino.SAMConfig();

  Serial.println("Waiting for a Mifare Ultralight tag to read.");
}

void blinkLikeHell(void) {
  for (int i = 0; i < 3; i++) {
    keyDuino.led(100);
    delay(100);
  }
}

void loop(void) {
  uint8_t success;
  success = keyDuino.readTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    if (uidLength == 7) {
      Serial.print("Found chip: ");
      keyDuino.PrintHex(uid, uidLength);

      Serial.println("");
      if (
        keyDuino.convertUintToString(uid, uidLength) ==
        keyDuino.convertUintToString(correctTag, uidLength)
      ) {
        Serial.println("Chip Found!");
        blinkLikeHell();
      } else {
        Serial.println("Unknown chip");
      }
      Serial.println("");
      delay(500);

    } else {
      Serial.println("Detected tag is not Mifare Ultralight.");
      delay(500);
    }
  }
}
