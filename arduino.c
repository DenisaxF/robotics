#include <Arduino.h> //header ce conține toate definițiile pentru API
#define RED_LED 13 // este de preferat în continuare să folosim macrouri, nu hardcodăm valori!
 
void setup() {
  pinMode(RED_LED, OUTPUT);
}
 
void loop() {
  digitalWrite(RED_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, LOW);
  delay(500);
}
