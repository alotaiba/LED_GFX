# LED_GFX

## HT1635B LED Matrix Text Effects Library for Particle

LED_GFX is a library for Particle devices. It allows the user to easily write text and add effects, such as scrolling and fading, to an LED matrix display that uses [HT1635B chip](http://www.holtek.com.tw/documents/10179/11842/HT1635v120.pdf). It supports callbacks to user defined functions when a certain effect, for example, has completed. It doesn't use any delays, so it can run more efficiently and in parallel with other processes.

This library uses [HT1635B](https://github.com/alotaiba/HT1635B), as its base for its internal communication with HT1635B chip.

# Usage

```Arduino
#include "LED_GFX.h"

Sequence seq1;
const int rows = 41;
const int columns = 7;
const int brightness = 6;

LED_GFX gfx(rows, columns, brightness);

void setup() {
  gfx.begin();

  seq.setText("Hello", alignmentCenter);
  seq.setAnimationIn(effectScrollInLeft, 60, 3000);
  seq.setAnimationOut(effectScrollOutRight, 60, 0);

  gfx.addSequence(seq);
  gfx.display();
}

void loop() {
  gfx.run();
}
```

See the examples folder for more details.

# Documentation

There are two main classes that are needed to interact with LED matrix display. [`Sequence`](#sequence) and [`LED_GFX`](#led_gfxrows-columns-brightness). `Sequence` is used for specifying the text to be displayed, its in and out effects, speed to which it moves, and pause. On the other hand, `LED_GFX`, takes all `Sequence` objects and queue them for display.

## `Sequence`
Creates a new `Sequence` class to manage text, effect, speed, etc.

```Arduino
#include "LED_GFX.h"
...
...
Sequence seq;
...
...
void setup() {
  seq.setText("Top", alignmentCenter);
  seq.setAnimationIn(effectScrollInTop, 60, 3000);
  seq.setAnimationOut(effectScrollOutBottom, 60, 0);
}
```

### Effects
| Name | Description |
|------|-------------|
| `effectNotSet` | Effect has not been set for animation, thus, the animation will not be shown on the display. This is the default case when the user doesn't specify an animation in or animation out. |
| `effectDisplay` | Displays text immediately without any animation. |
| `effectClear` | Clears text immediately without any animation. |
| `effectScrollInLeft` | Scrolls in text from the left. |
| `effectScrollInRight` | Scrolls in text from the right. |
| `effectScrollInBottom` | Scrolls in text from the bottom. |
| `effectScrollInTop` | Scrolls in text from the top. |
| `effectScrollOutLeft` | Scrolls out text to the left. |
| `effectScrollOutRight` | Scrolls out text to the right. |
| `effectScrollOutBottom` | Scrolls out text to the bottom. |
| `effectScrollOutTop` | Scrolls out text to the top. |
| `effectFadeIn` | Fades in text, from 0 to the pre-set brightness level. |
| `effectFadeOut` | Fades out text, from the pre-set brightness level to 0. |

### Text Alignments
| Name | Description |
|------|-------------|
| `alignmentLeft` | Aligns text to the left, this is the default when `alignment` in `setText()` is not set. |
| `alignmentCenter` | Aligns text to the center. |
| `alignmentRight` | Aligns text to the right. |

### `setText(text, [alignment])`
Specifies the text to be displayed on the LED matrix.

#### Parameters:
| Name | Type | Description |
|------|------|-------------|
| `text` | `const char*` | The actual text to be displayed. |
| `alignment` | `alignment_t` | [Text alignment](#text-alignments). Defaults to `alignmentLeft`. |

```Arduino
Sequence seq;

void setup() {
  seq.setText("Hello", alignmentCenter);
}
```

### `setAnimationIn(effect, [speed], [pause])`
Specifies the type of animation for the text coming in.

#### Parameters:
| Name | Type | Description |
|------|------|-------------|
| `effect` | `effect_t` | [Type of animation](#effects) for the text. |
| `speed` | `uint16_t` | Animation speed. It's actually the pause between each frame, in ms. Calling it speed is easier. Defaults to `0`. |
| `pause` | `uint16_t` | Pause, in ms, between the end of this animation, and the beginning of the next one. Defaults to `0`. |

```Arduino
Sequence seq;

void setup() {
  seq.setAnimationIn(effectScrollInLeft, 60, 3000);
}
```

### `setAnimationOut(effect, [speed], [pause])`
Specifies the type of animation for the text going out.

#### Parameters:
| Name | Type | Description |
|------|------|-------------|
| `effect` | `effect_t` | Type of animation for the text. |
| `speed` | `uint16_t` | Animation speed. It's actually the pause between each frame, in ms. Calling it speed is easier. Defaults to `0`. |
| `pause` | `uint16_t` | Pause, in ms, between the end of this animation, and the beginning of the next one. Defaults to `0`. |

```Arduino
Sequence seq;

void setup() {
  seq.setAnimationOut(effectScrollOutRight, 60, 3000);
}
```

### `onAnimationInDone(cb)`
Sets the user callback function, which should be called when the animation for the text coming in is done.

#### Parameters:
| Name | Type | Description |
|------|------|-------------|
| `cb` | `CallbackType` | User callback function. |

```Arduino
Sequence seq;

void setup() {
  seq.setText("Hello");
  seq.setAnimationIn(effectScrollInRight, 60, 3000);
  seq.setAnimationOut(effectScrollOutLeft, 60, 0);
  seq.onAnimationInDone(seqInDone);
}

void seqInDone() {
  Serial.println("Sequence animation in is done!");
}
```

### `onAnimationOutDone(cb)`
Sets the user callback function, which should be called when the animation for the text going out is done.

#### Parameters:
| Name | Type | Description |
|------|------|-------------|
| `cb` | `CallbackType` | User callback function. |

```Arduino
Sequence seq;

void setup() {
  seq.setText("Hello");
  seq.setAnimationIn(effectScrollInRight, 60, 3000);
  seq.setAnimationOut(effectScrollOutLeft, 60, 0);
  seq.onAnimationOutDone(seqOutDone);
}

void seqOutDone() {
  Serial.println("Sequence animation out is done!");
}
```

## `LED_GFX(rows, columns, [brightness])`
Creates a new `LED_GFX` class to manage one or multiple `Sequence` effects, and to manage the display of those effects. It sets up the display with appropriate number of rows, columns, and brightness level.

#### Parameters:
| Name | Type | Description |
|------|------|-------------|
| `rows` | `uint8_t` | Number of LED matrix rows. |
| `columns` | `uint8_t` | Number of LED matrix columns. |
| `brightness` | `uint8_t` | Brightness level from `0` low brightness to `15` highest brightness. Defaults to `6`. |

```Arduino
#include "LED_GFX.h"
...
...
LED_GFX gfx(rows, columns, brightness);
...
...
void setup() {
  gfx.begin();
  ...
  gfx.addSequence(seq);
  ...
  gfx.display();
}

void loop() {
  gfx.run();
}
```

### `begin(i2caddr)`
Initializes HT1635B chip and performs initial I2C setup. This method should be called before any others are used.

#### Parameters:
| Name | Type | Description |
|------|------|-------------|
| `i2caddr` | `uint8_t` | I2C address of the HT1635B device. Defaults to [HT1635B's library's](https://github.com/alotaiba/HT1635B) default. |

```Arduino
const int rows = 41;
const int columns = 7;
const int brightness = 6;

LED_GFX gfx(rows, columns, brightness);

void setup() {
  gfx.begin(0x68);
}
```

### `addSequence(sequence)`
Adds a `Sequence` instance to the internal queue for display later.

#### Parameters:
| Name | Type | Description |
|------|------|-------------|
| `sequence` | `Sequence` | Adds a `Sequence` instance to an internal array to be displayed when `display()` is called. |

```Arduino
Sequence seq;

const int rows = 41;
const int columns = 7;
const int brightness = 6;

LED_GFX gfx(rows, columns, brightness);

void setup() {
  gfx.begin();

  seq.setText("Hello");
  seq.setAnimationIn(effectScrollInLeft, 60, 3000);
  seq.setAnimationOut(effectScrollOutRight, 60, 0);

  gfx.addSequence(seq);
}
```

### `onDone(cb)`
Sets the user callback function, which should be called when the all sequences are done animating either in or out.

#### Parameters:
| Name | Type | Description |
|------|------|-------------|
| `cb` | `CallbackType` | User callback function. |

```Arduino
void setup() {
  gfx.begin();

  seq.setText("Hello");
  seq.setAnimationIn(effectScrollInLeft, 60, 3000);
  seq.setAnimationOut(effectScrollOutRight, 60, 0);

  gfx.addSequence(seq);

  gfx.onDone(allDone);
}

void loop() {
  gfx.run();
}

void allDone() {
  Serial.println("Done with all animations!");
}
```

### `display()`
Tells the library that all `Sequence` effects have been added to the internal queue, and should be displayed the next time `run()` is called. *Note: that if any more effects are added later, this function must be called again.*

#### Parameters:
None.

```Arduino
void setup() {
  gfx.begin();

  seq.setText("Hello");
  seq.setAnimationIn(effectScrollInLeft, 60, 3000);
  seq.setAnimationOut(effectScrollOutRight, 60, 0);

  gfx.addSequence(seq);

  gfx.display();
}

void loop() {
  gfx.run();
}
```

### `run()`
Since the library doesn't use any delays to time the effects, this function takes care of timing the effects. This function must be called as frequently and as fast as possible for accurate timing. Preferably, in the main `loop()`.

#### Parameters:
None.

```Arduino
void setup() {
  gfx.begin();

  seq.setText("Hello");
  seq.setAnimationIn(effectScrollInLeft, 60, 3000);
  seq.setAnimationOut(effectScrollOutRight, 60, 0);

  gfx.addSequence(seq);

  gfx.display();
}

void loop() {
  gfx.run();
}
```

# LICENSE
Copyright 2017 Abdulrahman Saleh Khamis

Licensed under the MIT license
