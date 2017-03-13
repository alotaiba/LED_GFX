#include "LED_GFX.h"

Sequence seq1, seq2;
const int brightness = 6;
const int rows = 41;
const int columns = 7;

LED_GFX gfx(rows, columns, brightness);

SYSTEM_THREAD(ENABLED);

void setup() {
  Serial.begin(115200);
  gfx.begin();

  seq1.setText("Hello", alignmentCenter);
  seq1.setAnimationIn(effectScrollInRight, 60, 3000);
  seq1.setAnimationOut(effectScrollOutRight, 60, 0);
  seq1.onAnimationInDone(seq1InDone);
  seq1.onAnimationOutDone(seq1OutDone);

  seq2.setText("World", alignmentCenter);
  seq2.setAnimationIn(effectScrollInLeft, 60, 3000);
  seq2.setAnimationOut(effectScrollOutLeft, 60, 0);
  seq2.onAnimationInDone(seq2InDone);
  seq2.onAnimationOutDone(seq2OutDone);

  gfx.addSequence(seq1);
  gfx.addSequence(seq2);

  gfx.onDone(allDone);
  gfx.display();
}

void loop() {
  gfx.run();
}

void seq1InDone() {
  Serial.println("Sequence 1 in animation is done!");
}

void seq1OutDone() {
  Serial.println("Sequence 1 out animation is done!");
}

void seq2InDone() {
  Serial.println("Sequence 2 in animation is done!");
}

void seq2OutDone() {
  Serial.println("Sequence 2 out animation is done!");
}

void allDone() {
  Serial.println("Done with all animations!");
}
