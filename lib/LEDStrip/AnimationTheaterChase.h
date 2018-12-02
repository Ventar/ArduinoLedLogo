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
  AnimationTheaterChase(LogoConfig* config, Adafruit_NeoPixel* strip)
      : Animation(config, strip, "Theater Chase", "Strip", "theater"){};

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