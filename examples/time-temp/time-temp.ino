#include "LED_GFX.h"

Sequence seq1, seq2;
char timeString[80];
unsigned long startedMillis;
unsigned long lastDisplayTempMillis = 0;
unsigned long lastDisplayTimeMillis = 0;
int lastSecond = 0;
boolean displayTime = true;
boolean displayTemp = false;
int displayTimeCount = 0;
int displayTempCount = 0;
const int brightness = 6;
const int rows = 41;
const int columns = 7;

LED_GFX gfx(rows, columns, brightness);

SYSTEM_THREAD(ENABLED);

void setup() {
  Serial.begin(115200);
  gfx.begin();
  startedMillis = millis();
  lastSecond = Time.second();
  displayTime = true;
}

void loop() {
  gfx.run();
  unsigned long now = millis();

  if (displayTime) {
    if (Time.second() != lastSecond) {
      lastSecond = Time.second();
      displayTimeLED();
      displayTimeCount++;
    } else if (displayTimeCount >= 10) {
      delay(800); //To avoid switching to temperature display at the start of a rolling second
      displayTime = false;
      displayTimeCount = 0;
    }
  } else {
    if (now - lastDisplayTempMillis >= 1000) {
      lastDisplayTempMillis = now;
      displayTempLED();
      displayTempCount++;
    } else if (displayTempCount >= 3) {
      displayTime = true;
      displayTempCount = 0;
    }
  }
}

void displayTimeLED() {
  setTime(timeString);
  seq1.setText(timeString, alignmentCenter);
  seq1.setAnimationIn(effectDisplay);
  gfx.addSequence(seq1);
  gfx.display();
}

void displayTempLED() {
  seq2.setText("25 C", alignmentCenter);
  seq2.setAnimationIn(effectDisplay);
  gfx.addSequence(seq2);
  gfx.display();
}

void setTime(char *time) {
  char buffer[1];
  long now = millis() - startedMillis;
  int elapsedHours = 0;
  int elapsedMinutes = 0;
  int elapsedSeconds = 0;
  int elapsedDisplay = 0;

  elapsedHours = int((now / 3600000) % 99);
  elapsedMinutes = int((now / 60000) % 60);
  elapsedSeconds = int((now / 1000) % 60);
  elapsedDisplay = elapsedHours * 10000 + elapsedMinutes * 100 + elapsedSeconds;

  strcpy(time, "");

  //Hour
  //itoa(elapsedDisplay/100000 % 10, buffer, 10);
  //strcat(time, buffer);
  //itoa(elapsedDisplay/10000 % 10, buffer, 10);
  //strcat(time, buffer);

  //strcat(time, ":");

  //Minute
  itoa(elapsedDisplay/1000 % 10, buffer, 10);
  strcat(time, buffer);
  itoa(elapsedDisplay/100 % 10, buffer, 10);
  strcat(time, buffer);

  strcat(time, ":");

  //Second
  itoa(elapsedDisplay/10 % 10, buffer, 10);
  strcat(time, buffer);
  itoa(elapsedDisplay % 10, buffer, 10);
  strcat(time, buffer);
}
