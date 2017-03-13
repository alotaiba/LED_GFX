#include "Sequence.h"

/**
 * Constructor.
 */
Sequence::Sequence() {
  //Defaults
  alignment_ = alignmentLeft;
  effectIn_ = effectNotSet;
  effectOut_ = effectNotSet;
  speedIn_ = 0;
  speedOut_ = 0;
  pauseIn_ = 0;
  pauseOut_ = 0;
}

void Sequence::setText(const char *text, alignment_t alignment) {
  text_ = text;
  alignment_ = alignment;
}

void Sequence::setAnimationIn(effect_t effect, uint16_t speed, uint16_t pause) {
  effectIn_ = effect;
  speedIn_ = speed;
  pauseIn_ = pause;
}

void Sequence::setAnimationOut(effect_t effect, uint16_t speed, uint16_t pause) {
  effectOut_ = effect;
  speedOut_ = speed;
  pauseOut_ = pause;
}

const char *Sequence::getText() {
  return text_;
}

alignment_t Sequence::getAlignment() {
  return alignment_;
}

effect_t Sequence::getEffectIn() {
  return effectIn_;
}

effect_t Sequence::getEffectOut() {
  return effectOut_;
}

uint16_t Sequence::getSpeedIn() {
  return speedIn_;
}

uint16_t Sequence::getSpeedOut() {
  return speedOut_;
}

uint16_t Sequence::getPauseIn() {
  return pauseIn_;
}

uint16_t Sequence::getPauseOut() {
  return pauseOut_;
}

Sequence::CallbackType Sequence::getAnimationInDoneCallback() {
  return animationInCb_;
}

Sequence::CallbackType Sequence::getAnimationOutDoneCallback() {
  return animationOutCb_;
}

void Sequence::onAnimationInDone(CallbackType cb) {
  animationInCb_ = cb;
}

void Sequence::onAnimationOutDone(CallbackType cb) {
  animationOutCb_ = cb;
}
