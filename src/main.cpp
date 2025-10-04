#include <Arduino.h>
#include <avr/wdt.h>

#define FRAME_LENGTH 4

unsigned long last_tick = 0;
uint8_t current_step = 0;

void setup() {
  wdt_disable();
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
}

void RTWFlash(uint8_t pin) {
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

void FadeIn(uint8_t pin) {
  for (uint8_t i = 0; i <= 0xFF; i++) {
    analogWrite(pin, i);
    delay(5);
    yield();
  }
}

void FadeOut(uint8_t pin) {
  for (uint8_t i = 0xFF; i >= 0; i--) {
    analogWrite(pin, i);
    delay(5);
    yield();
  }
}

void flashAB(unsigned long delay_ms) {
  digitalWrite(1, LOW);
  digitalWrite(0, HIGH);
  delay(delay_ms);
  digitalWrite(0, LOW);
  digitalWrite(1, HIGH);
  delay(delay_ms);
}

void walk(unsigned long delay_ms) {
  digitalWrite(0, LOW);
  digitalWrite(1, LOW);
  delay(delay_ms);
  digitalWrite(0, LOW);
  digitalWrite(1, HIGH);
  delay(delay_ms);
  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);
  delay(delay_ms);
  digitalWrite(0, HIGH);
  digitalWrite(1, LOW);
  delay(delay_ms);
}

void walk_rev(unsigned long delay_ms) {
  digitalWrite(0, LOW);
  digitalWrite(1, LOW);
  delay(delay_ms);
  digitalWrite(0, HIGH);
  digitalWrite(1, LOW);
  delay(delay_ms);
  digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);
  delay(delay_ms);
  digitalWrite(0, LOW);
  digitalWrite(1, HIGH);
  delay(delay_ms);
}

void tick_step() {
  if (current_step == 0xFF){
    current_step = 0;
  }

  analogWrite(0, current_step);
  analogWrite(1, 0xFF - current_step);

  current_step++;
}

void tick() {
  if (millis() >= last_tick + 5){
    tick_step();
    last_tick = millis();
  }
}

void loop() {
  tick();
}
