#pragma once

/* lED_GFX library by Abdulrahman Saleh Khamis
 */

#include "Particle.h"
#include "HT1635B.h"
#include "Sequence.h"

#define LED_GFX_MAX_SEQUENCE_QUEUE    25
#define LED_GFX_MAX_ROWS              44
#define LED_GFX_MAX_COLUMNS           8
#define LED_GFX_FONT_WIDTH            5  //5 rows per char
#define LED_GFX_FONT_HEIGHT           7  //7 columns per char
#define LED_GFX_MAX_CHARACTERS        32 //Maximum number of characters for an animation

// This is your main class that users will import into their application
class LED_GFX
{
public:
  typedef void (*CallbackType)();
  /**
   * Constructor
   */
  LED_GFX(uint8_t rows, uint8_t columns, uint8_t brightness = 6);
  void begin(uint8_t i2caddr = HT1635B_I2C_ADDRESS);
  void addSequence(Sequence sequence);
  void onDone(CallbackType cb);
  void display();
  void run();

private:
  typedef struct {
    effect_t effect;
    alignment_t alignment;
    const char *text;
    uint16_t speed;
    uint16_t pause;
    uint8_t totalFrames;
    uint8_t currentFrame;
    Sequence::CallbackType callback;
  } animation_t;

  Sequence sequenceQueue_[LED_GFX_MAX_SEQUENCE_QUEUE];
  animation_t animationQueue_[LED_GFX_MAX_SEQUENCE_QUEUE*2];
  size_t sequenceQueueCount_;
  size_t animationQueueCount_;
  size_t currentAnimation_;
  unsigned long lastAnimationPauseTime_;
  unsigned long lastAnimationSpeedTime_;
  boolean display_;
  uint8_t rows_;
  uint8_t columns_;
  uint8_t brightness_;
  size_t maxCharacters_;
  CallbackType cb_;


  //Internal helper methods
  void drawText_(const char *text, int xOffSet, int yOffSet);
  void displayFrame_(animation_t animation);
  uint8_t getAlignmentFrameAdjustment_(animation_t animation);
  uint8_t getTotalFramesForEffect_(animation_t animation);
  void addAnimation_(animation_t animation);
  void reset_();
  size_t getMaximumCharacters_();

  //Effects
  void scrollInRight_(animation_t animation);
  void scrollInLeft_(animation_t animation);
  void scrollInBottom_(animation_t animation);
  void scrollInTop_(animation_t animation);
  void scrollOutRight_(animation_t animation);
  void scrollOutLeft_(animation_t animation);
  void scrollOutBottom_(animation_t animation);
  void scrollOutTop_(animation_t animation);
  void fadeIn_(animation_t animation);
  void fadeOut_(animation_t animation);
  void justDisplay_(animation_t animation);
  void justClear_();
};
