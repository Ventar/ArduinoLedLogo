#ifndef AnimationTheaterChase_h
#define AnimationTheaterChase_h

#include <Animation.h>

/**
 * Theater Chase animation.
 */
class AnimationTheaterChase : public Animation {
 public:
  /**
   * Constructor.
   */
  AnimationTheaterChase(Adafruit_NeoPixel* strip)
      : Animation(strip, THEATER_CHASE, "theater"){};

  /**
   * Resets the animation to the initial values.
   */
  void reset() override;

  /**
   * Performs all update operations of the LEDs when the underlying delay is
   * passed.
   */
  void process() override;
};

#endif