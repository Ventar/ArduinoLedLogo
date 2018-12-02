#ifndef AnimationStatic_h
#define AnimationStatic_h

#include <Animation.h>

class AnimationStatic : public Animation {
 public:
  /**
   * Constructor
   */
  AnimationStatic(LogoConfig* config, Adafruit_NeoPixel* strip)
      : Animation(config, strip, "Static", "Strip", "static"){};

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