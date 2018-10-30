#include <KeyDuino.h>
#include <config.h>

KeyDuino keyDuino;

uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
uint8_t uidLength;

void setup(void) {
	Serial.begin(115200);
	keyDuino.begin();
	keyDuino.SAMConfig();
	pinMode(PIN_RELAY, OUTPUT);
	digitalWrite(PIN_RELAY, LOW);
	Serial.println("Ready");
}

void blinkFail(void) {
	for (int i = 0; i < 3; i++) {
		keyDuino.led(100);
		delay(100);
	}
}

void startUnlock(void) {
	digitalWrite(PIN_RELAY, HIGH);
	keyDuino.led(1000);
}

void stopUnlock(void) {
	digitalWrite(PIN_RELAY, LOW);
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
				startUnlock();
				stopUnlock();
			} else {
				Serial.println("Unknown chip");
				blinkFail();
			}
			Serial.println("");
			delay(500);

		} else {
			Serial.println("Detected tag is not Mifare Ultralight.");
			blinkFail();
			delay(500);
		}
	}
}
