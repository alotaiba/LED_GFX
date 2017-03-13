#pragma once

#include "Particle.h"

typedef enum {
  effectNotSet,
  effectDisplay,
  effectClear,
  effectScrollInLeft,
  effectScrollInRight,
  effectScrollInBottom,
  effectScrollInTop,
  effectScrollOutLeft,
  effectScrollOutRight,
  effectScrollOutBottom,
  effectScrollOutTop,
  effectFadeIn,
  effectFadeOut
} effect_t;

typedef enum {
  alignmentLeft,
  alignmentCenter,
  alignmentRight
} alignment_t;

class Sequence
{
  public:
    typedef void (*CallbackType)();
    /**
     * Constructor
     */
    Sequence();

    void setText(const char *text, alignment_t alignment = alignmentLeft);
    void setAnimationIn(effect_t effect, uint16_t speed = 0, uint16_t pause = 0);
    void setAnimationOut(effect_t effect, uint16_t speed = 0, uint16_t pause = 0);
    void onAnimationInDone(CallbackType cb);
    void onAnimationOutDone(CallbackType cb);

    const char *getText();
    alignment_t getAlignment();
    effect_t getEffectIn();
    effect_t getEffectOut();
    uint16_t getSpeedIn();
    uint16_t getSpeedOut();
    uint16_t getPauseIn();
    uint16_t getPauseOut();
    Sequence::CallbackType getAnimationInDoneCallback();
    Sequence::CallbackType getAnimationOutDoneCallback();

  private:
    const char *text_;
    alignment_t alignment_;
    effect_t effectIn_;
    effect_t effectOut_;
    uint16_t speedIn_;
    uint16_t speedOut_;
    uint16_t pauseIn_;
    uint16_t pauseOut_;
    CallbackType animationInCb_;
    CallbackType animationOutCb_;
};
