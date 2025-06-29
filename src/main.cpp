#include <Arduino.h>

#define FRAME_LENGTH 4

void setup() {
  pinMode(1, OUTPUT);
}

void flash(uint8_t pin) {
  digitalWrite(pin, HIGH);
  delay(41 * FRAME_LENGTH);
  digitalWrite(pin, LOW);
  delay(14 * FRAME_LENGTH);
  digitalWrite(pin, HIGH);
  delay(6 * FRAME_LENGTH);
  digitalWrite(pin, LOW);
  delay(3 * FRAME_LENGTH);
  digitalWrite(pin, HIGH);
  delay(6 * FRAME_LENGTH);
  digitalWrite(pin, LOW);
  delay(37 * FRAME_LENGTH);
}

void loop() {
  flash(1);
  flash(2);
}
