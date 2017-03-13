#include "LED_GFX.h"

Sequence seq1, seq2, seq3, seq4, seq5, seq6;
const int rows = 41;
const int columns = 7;
const int brightness = 6;

LED_GFX gfx(rows, columns, brightness);

SYSTEM_THREAD(ENABLED);

void setup() {
  gfx.begin();

  seq1.setText("R >", alignmentCenter);
  seq1.setAnimationIn(effectScrollInLeft, 60, 3000);
  seq1.setAnimationOut(effectScrollOutRight, 60, 0);

  seq2.setText("< L", alignmentCenter);
  seq2.setAnimationIn(effectScrollInRight, 60, 3000);
  seq2.setAnimationOut(effectScrollOutLeft, 60, 0);

  seq3.setText("v T", alignmentCenter);
  seq3.setAnimationIn(effectScrollInTop, 60, 3000);
  seq3.setAnimationOut(effectScrollOutBottom, 60, 0);

  seq4.setText("^ B", alignmentCenter);
  seq4.setAnimationIn(effectScrollInBottom, 60, 3000);
  seq4.setAnimationOut(effectScrollOutTop, 60, 0);

  seq5.setText("Fade", alignmentCenter);
  seq5.setAnimationIn(effectFadeIn, 70, 3000);
  seq5.setAnimationOut(effectFadeOut, 70, 300);

  seq6.setText("Bye!", alignmentCenter);
  seq6.setAnimationIn(effectDisplay, 0, 3000);
  seq6.setAnimationOut(effectClear);

  gfx.addSequence(seq1);
  gfx.addSequence(seq2);
  gfx.addSequence(seq3);
  gfx.addSequence(seq4);
  gfx.addSequence(seq5);
  gfx.addSequence(seq6);

  gfx.display();
}

void loop() {
  gfx.run();
}
