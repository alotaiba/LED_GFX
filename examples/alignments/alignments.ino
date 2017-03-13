#include "LED_GFX.h"

Sequence seq1, seq2, seq3;
const int brightness = 6;
const int rows = 41;
const int columns = 7;

LED_GFX gfx(rows, columns, brightness);

SYSTEM_THREAD(ENABLED);

void setup() {
  gfx.begin();

  seq1.setText("Left", alignmentLeft);
  seq1.setAnimationIn(effectDisplay, 0, 3000);
  seq1.setAnimationOut(effectClear);

  seq2.setText("Cntr", alignmentCenter);
  seq2.setAnimationIn(effectDisplay, 0, 3000);
  seq2.setAnimationOut(effectClear);

  seq3.setText("Rght", alignmentRight);
  seq3.setAnimationIn(effectDisplay, 0, 3000);
  seq3.setAnimationOut(effectClear);

  gfx.addSequence(seq1);
  gfx.addSequence(seq2);
  gfx.addSequence(seq3);

  gfx.display();
}

void loop() {
  gfx.run();
}
