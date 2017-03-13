/* lED_GFX library by Abdulrahman Saleh Khamis
 */

#include "LED_GFX.h"
#include "font.h"

HT1635B LEDMatrix;

/**
 * Constructor.
 */
LED_GFX::LED_GFX(uint8_t rows, uint8_t columns, uint8_t brightness) {
  rows_ = rows;
  columns_ = columns;
  brightness_ = brightness;
  maxCharacters_ = getMaximumCharacters_();
  reset_();
}

void LED_GFX::begin(uint8_t i2caddr) {
  LEDMatrix.begin(i2caddr);
  LEDMatrix.setBrightness(brightness_);
}

void LED_GFX::addSequence(Sequence sequence) {
  sequenceQueue_[sequenceQueueCount_] = sequence;
  sequenceQueueCount_++;
}

void LED_GFX::onDone(CallbackType cb) {
  cb_ = cb;
}

void LED_GFX::display() {
  if (sequenceQueueCount_ <= 0) {
    display_ = false;
    return;
  }

  for (size_t i = 0; i < sequenceQueueCount_; i++) {
    animation_t animation;
    animation.alignment = sequenceQueue_[i].getAlignment();
    animation.text = sequenceQueue_[i].getText();
    animation.currentFrame = 1;
    for (size_t j = 0; j < 2; j++) {
      if (j == 0) {
        animation.effect = sequenceQueue_[i].getEffectIn();
        animation.speed = sequenceQueue_[i].getSpeedIn();
        animation.pause = sequenceQueue_[i].getPauseIn();
        animation.callback = sequenceQueue_[i].getAnimationInDoneCallback();
        animation.totalFrames = getTotalFramesForEffect_(animation);
        if (animation.effect != effectNotSet) {
          addAnimation_(animation);
        }
      } else {
        animation.effect = sequenceQueue_[i].getEffectOut();
        animation.speed = sequenceQueue_[i].getSpeedOut();
        animation.pause = sequenceQueue_[i].getPauseOut();
        animation.callback = sequenceQueue_[i].getAnimationOutDoneCallback();
        animation.totalFrames = getTotalFramesForEffect_(animation);
        if (animation.effect != effectNotSet) {
          addAnimation_(animation);
        }
      }
    }
  }

  display_ = true;
}

void LED_GFX::run() {
  //Don't display anything if there's nothing to be displayed
  if (!display_)
    return;

  unsigned long currentMillis = millis();

  if (currentAnimation_ < animationQueueCount_) {
    animation_t animation = animationQueue_[currentAnimation_];
    boolean animationPauseFinished = (currentMillis - lastAnimationPauseTime_ >= animation.pause);
    boolean framePauseFinished = (currentMillis - lastAnimationSpeedTime_ >= animation.speed);
    boolean animationFinished = (animation.currentFrame > animation.totalFrames);

    if (!animationFinished && framePauseFinished) {
      //Display the next frame of the animation
      displayFrame_(animation);
      animationQueue_[currentAnimation_].currentFrame++;
      lastAnimationSpeedTime_ = lastAnimationPauseTime_ = currentMillis;
    }

    if (animationFinished && animationPauseFinished) {
      //Call the user's feedback if it's been set
      if (animation.callback) {
        animation.callback();
      }
      //Advance the counter to the next animation
      currentAnimation_++;
      lastAnimationPauseTime_ = currentMillis;
    }
  } else {
    //Reset everything when the animation sequences are done,
    //then call the user's callback function.
    reset_();
    if (cb_) {
      cb_();
    }
  }
}

void LED_GFX::reset_() {
  memset(sequenceQueue_, 0, sizeof(sequenceQueue_));
  memset(animationQueue_, 0, sizeof(animationQueue_));
  sequenceQueueCount_ = 0;
  animationQueueCount_ = 0;
  currentAnimation_ = 0;
  lastAnimationPauseTime_ = 0;
  lastAnimationSpeedTime_ = 0;
  display_ = false;
}

size_t LED_GFX::getMaximumCharacters_() {
  int count = rows_;
  uint8_t i = 0;
  while (count >= LED_GFX_FONT_WIDTH) {
    i++;
    count -= LED_GFX_FONT_WIDTH + 1; //account for a spacer row
  }
  return i;
}

void LED_GFX::drawText_(const char *text, int xOffSet, int yOffSet)
{
  unsigned int byteBuffer[LED_GFX_MAX_CHARACTERS];

  for (size_t i = 0; i < strlen(text); i++) {
    //Get the letter's first element position.
    byteBuffer[i] = (int)text[i]; //Get decimal representation of the letter, e.g. A = 65
    byteBuffer[i] = (byteBuffer[i] - 32) * LED_GFX_FONT_WIDTH; //Subtract 32 and multiply by 5 bytes width to reach the first element
    //e.g. A = 165, font.h has the letter A, and its first element is '0x7E' at position 165.
  }

  //Print each letter in text
  for (size_t i = 0; i < strlen(text); i++) {
    //Five lines (0-4) with fixed width font
    for (size_t j = 0; j < LED_GFX_FONT_WIDTH; j++) {
      int index = (i * (LED_GFX_FONT_WIDTH + 1)) + j + xOffSet; //account for the spacer row and xOffset
      int data = 0;
      //ROW memory is 8 bits, since we're only using 7 bits, we need to account for the extra bit
      //and shift the yOffSet accordingly.
      if (yOffSet >= 0) {
        data = ((Font5x7[byteBuffer[i] + j]) << (LED_GFX_MAX_COLUMNS - LED_GFX_FONT_HEIGHT + yOffSet));
      } else {
        data = ((Font5x7[byteBuffer[i] + j]) >> (abs(yOffSet) - LED_GFX_MAX_COLUMNS + LED_GFX_FONT_HEIGHT));
      }
      if (index >= 0 && index < rows_) {
        LEDMatrix.setRowMemory(index, data);
      }
    }
  }
  LEDMatrix.render();

}


void LED_GFX::addAnimation_(animation_t animation) {
  animationQueue_[animationQueueCount_] = animation;
  animationQueueCount_++;
}


uint8_t LED_GFX::getTotalFramesForEffect_(animation_t animation) {
  uint8_t totalFrames = 0;
  uint8_t alignmentAdjustment = getAlignmentFrameAdjustment_(animation);
  switch (animation.effect) {
    case effectScrollInRight:
    case effectScrollOutRight:
      totalFrames = rows_ - alignmentAdjustment;
      break;
    case effectScrollInLeft:
    case effectScrollOutLeft:
      //char length * char font rows + spaces between letters + alignment space
      totalFrames = (strlen(animation.text) * LED_GFX_FONT_WIDTH) + (strlen(animation.text) - 1) + alignmentAdjustment;
      break;
    case effectScrollInBottom:
    case effectScrollOutBottom:
    case effectScrollInTop:
    case effectScrollOutTop:
      totalFrames = columns_;
      break;
    case effectFadeIn:
      //+1 to cover levels 0 to brightness_
      totalFrames = brightness_ + 1;
      break;
    case effectFadeOut:
      //+1 to cover levels 0 to brightness_
      //+1 to clear the display at the end
      totalFrames = brightness_ + 2;
      break;
    case effectDisplay:
    case effectClear:
    default:
      totalFrames = 1;
      break;
  }
  return totalFrames;
}

uint8_t LED_GFX::getAlignmentFrameAdjustment_(animation_t animation) {
  uint8_t adjustment = 0;
  if (   strlen(animation.text) <= 0
      || strlen(animation.text) >= maxCharacters_
      || animation.alignment == alignmentLeft)
  {
    return adjustment;
  }

  switch (animation.alignment) {
    case alignmentCenter:
      adjustment = ((LED_GFX_FONT_WIDTH + 1)/2) * (maxCharacters_ - strlen(animation.text));
      break;
    case alignmentRight:
      adjustment = (LED_GFX_FONT_WIDTH + 1) * (maxCharacters_ - strlen(animation.text));
      break;
  }

  return adjustment;
}

void LED_GFX::displayFrame_(animation_t animation) {
  switch (animation.effect) {
    case effectScrollInRight:
      scrollInRight_(animation);
      break;
    case effectScrollInLeft:
      scrollInLeft_(animation);
      break;
    case effectScrollInBottom:
      scrollInBottom_(animation);
      break;
    case effectScrollInTop:
      scrollInTop_(animation);
      break;
    case effectScrollOutRight:
      scrollOutRight_(animation);
      break;
    case effectScrollOutLeft:
      scrollOutLeft_(animation);
      break;
    case effectScrollOutBottom:
      scrollOutBottom_(animation);
      break;
    case effectScrollOutTop:
      scrollOutTop_(animation);
      break;
    case effectFadeIn:
      fadeIn_(animation);
      break;
    case effectFadeOut:
      fadeOut_(animation);
      break;
    case effectClear:
      justClear_();
      break;
    case effectDisplay:
    default:
      justDisplay_(animation);
      break;
  }
}

void LED_GFX::scrollInRight_(animation_t animation) {
  uint8_t alignmentAdjustment = getAlignmentFrameAdjustment_(animation);
  int offset = animation.totalFrames - animation.currentFrame + alignmentAdjustment;
  LEDMatrix.clear();
  drawText_(animation.text, offset, 0);
}

void LED_GFX::scrollInLeft_(animation_t animation) {
  uint8_t alignmentAdjustment = getAlignmentFrameAdjustment_(animation);
  int offset = animation.currentFrame - animation.totalFrames + alignmentAdjustment;
  LEDMatrix.clear();
  drawText_(animation.text, offset, 0);
}

void LED_GFX::scrollInBottom_(animation_t animation) {
  uint8_t alignmentAdjustment = getAlignmentFrameAdjustment_(animation);
  int offset = animation.totalFrames - animation.currentFrame;
  LEDMatrix.clear();
  drawText_(animation.text, alignmentAdjustment, offset);
}

void LED_GFX::scrollInTop_(animation_t animation) {
  uint8_t alignmentAdjustment = getAlignmentFrameAdjustment_(animation);
  int offset = animation.currentFrame - animation.totalFrames;
  LEDMatrix.clear();
  drawText_(animation.text, alignmentAdjustment, offset);
}

void LED_GFX::scrollOutLeft_(animation_t animation) {
  uint8_t alignmentAdjustment = getAlignmentFrameAdjustment_(animation);
  int offset = alignmentAdjustment - animation.currentFrame;
  LEDMatrix.clear();
  drawText_(animation.text, offset, 0);
}

void LED_GFX::scrollOutRight_(animation_t animation) {
  uint8_t alignmentAdjustment = getAlignmentFrameAdjustment_(animation);
  int offset = alignmentAdjustment + animation.currentFrame;
  LEDMatrix.clear();
  drawText_(animation.text, offset, 0);
}

void LED_GFX::scrollOutBottom_(animation_t animation) {
  uint8_t alignmentAdjustment = getAlignmentFrameAdjustment_(animation);
  int offset = animation.currentFrame;
  LEDMatrix.clear();
  drawText_(animation.text, alignmentAdjustment, offset);
}

void LED_GFX::scrollOutTop_(animation_t animation) {
  uint8_t alignmentAdjustment = getAlignmentFrameAdjustment_(animation);
  int offset = (-1) * animation.currentFrame;
  LEDMatrix.clear();
  drawText_(animation.text, alignmentAdjustment, offset);
}

void LED_GFX::fadeIn_(animation_t animation) {
  uint8_t alignmentAdjustment = getAlignmentFrameAdjustment_(animation);
  int level = animation.currentFrame - 1;
  LEDMatrix.clear();
  LEDMatrix.setBrightness(level);
  drawText_(animation.text, alignmentAdjustment, 0);
}

void LED_GFX::fadeOut_(animation_t animation) {
  uint8_t alignmentAdjustment = getAlignmentFrameAdjustment_(animation);
  int level = animation.totalFrames - animation.currentFrame - 1;
  if (level >= 0) {
    LEDMatrix.clear();
    LEDMatrix.setBrightness(level);
    drawText_(animation.text, alignmentAdjustment, 0);
  } else {
    LEDMatrix.clear();
    LEDMatrix.render();
    LEDMatrix.setBrightness(brightness_);
  }
}

void LED_GFX::justDisplay_(animation_t animation) {
  uint8_t alignmentAdjustment = getAlignmentFrameAdjustment_(animation);
  LEDMatrix.clear();
  drawText_(animation.text, alignmentAdjustment, 0);
}

void LED_GFX::justClear_() {
  LEDMatrix.clear();
  LEDMatrix.render();
}
