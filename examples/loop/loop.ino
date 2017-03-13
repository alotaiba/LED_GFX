#include "LED_GFX.h"

Sequence seq1, seq2, seq3, seq4;
const int brightness = 6;
const int rows = 41;
const int columns = 7;

LED_GFX gfx(rows, columns, brightness);

SYSTEM_THREAD(ENABLED);

void setup() {
  gfx.begin();
  gfx.onDone(display);
  //Kick-start the loop
  display();
}

void loop() {
  gfx.run();
}

void display() {
  seq1.setText("Ready", alignmentCenter);
  seq1.setAnimationIn(effectFadeIn, 70, 3000);
  seq1.setAnimationOut(effectFadeOut, 70, 300);

  seq2.setText("Hello", alignmentCenter);
  seq2.setAnimationIn(effectScrollInTop, 60, 2000);
  seq2.setAnimationOut(effectScrollOutBottom, 60, 0);

  seq3.setText("World!", alignmentCenter);
  seq3.setAnimationIn(effectScrollInBottom, 60, 2000);
  seq3.setAnimationOut(effectScrollOutTop, 60, 0);

  seq4.setText("This is a long text to scroll it.");
  seq4.setAnimationIn(effectScrollInRight, 60);
  seq4.setAnimationOut(effectScrollOutLeft, 60);

  gfx.addSequence(seq1);
  gfx.addSequence(seq2);
  gfx.addSequence(seq3);
  gfx.addSequence(seq4);

  gfx.display();
}
