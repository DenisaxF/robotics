#include <Arduino.h>

#define LED1 10
#define LED2 9
#define LED3 8
#define LED4 7
#define red 6
#define blue 5
#define green 4
#define buttonStop 3
#define buttonStart 2
#define apasatLung 1000

int ultimaStare = 0;
int stareCurenta;
unsigned long pressed_time = 0;
bool stopProces = false;
bool startApasat = false;
bool intrerupt = false;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(buttonStop, INPUT_PULLUP);
  pinMode(buttonStart, INPUT_PULLUP);
}

void delayWithStopCheck(unsigned long time) {
  unsigned long start = millis();
  while (millis() - start < time) {
    stareCurenta = digitalRead(buttonStop);
    if (stareCurenta == LOW && ultimaStare == HIGH) {
      pressed_time = millis();
      stopProces = false;
    }
    if (startApasat && stareCurenta == LOW && (millis() - pressed_time > apasatLung) && !stopProces) {
      intrerupt = true;  // Procesul este întrerupt
      return;
    }
    ultimaStare = stareCurenta;
  }
}

void clipire(int pin) {
  for (int i = 0; i <= 3; i++) {
    if (intrerupt) return;  
    digitalWrite(pin, !digitalRead(pin));
    delayWithStopCheck(1000); 
  }
  digitalWrite(pin, HIGH);
}

void clipire_toate() {
  for (int i = 0; i < 6 && !intrerupt; i++) {
    if (intrerupt) return; 
    digitalWrite(LED1, !digitalRead(LED1));
    digitalWrite(LED2, !digitalRead(LED2));
    digitalWrite(LED3, !digitalRead(LED3));
    digitalWrite(LED4, !digitalRead(LED4));
    delayWithStopCheck(500);  
  }
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
}

void loop() {
  intrerupt = false; 
  if (digitalRead(buttonStart) == LOW && !intrerupt) {
    startApasat = true;
    digitalWrite(red, HIGH);

    clipire(LED1);
    if (intrerupt) return; 

    clipire(LED2);
    if (intrerupt) return;

    clipire(LED3);
    if (intrerupt) return;

    clipire(LED4);
    if (intrerupt) return;

    clipire_toate();
    if (intrerupt) return;

    
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH); 
  }

  
  stareCurenta = digitalRead(buttonStop);
  if (stareCurenta == LOW && ultimaStare == HIGH) {
    pressed_time = millis();  
    stopProces = false;
  }

  if (startApasat && stareCurenta == LOW && (millis() - pressed_time > apasatLung) && !stopProces) {
    intrerupt = true; 
    clipire_toate();
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);  
    stopProces = true;
  }

  ultimaStare = stareCurenta;
}
