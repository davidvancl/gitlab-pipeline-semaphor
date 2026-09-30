#include "Semaphore.h"

#include <Arduino.h>
#include "config.h"

namespace Semaphore {

void begin() {
  pinMode(RED_RELAY_GPIO, OUTPUT);
  pinMode(YELLOW_RELAY_GPIO, OUTPUT);
  pinMode(GREEN_RELAY_GPIO, OUTPUT);
  off();
}

void off() {
  digitalWrite(RED_RELAY_GPIO, LOW);
  digitalWrite(YELLOW_RELAY_GPIO, LOW);
  digitalWrite(GREEN_RELAY_GPIO, LOW);
}

void yellow() {
  digitalWrite(RED_RELAY_GPIO, LOW);
  digitalWrite(YELLOW_RELAY_GPIO, HIGH);
  digitalWrite(GREEN_RELAY_GPIO, LOW);
}

void green() {
  digitalWrite(RED_RELAY_GPIO, LOW);
  digitalWrite(YELLOW_RELAY_GPIO, LOW);
  digitalWrite(GREEN_RELAY_GPIO, HIGH);
}

void red() {
  digitalWrite(RED_RELAY_GPIO, HIGH);
  digitalWrite(YELLOW_RELAY_GPIO, LOW);
  digitalWrite(GREEN_RELAY_GPIO, LOW);
}

}
