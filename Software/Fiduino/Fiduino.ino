#include "newVector.h"

void setup() {
  pinMode(13,OUTPUT);
  Vector<int> vect;
}

void loop() {
  digitalWrite(13,HIGH);
  delay(500);
  digitalWrite(13,LOW);
  delay(500);
}
