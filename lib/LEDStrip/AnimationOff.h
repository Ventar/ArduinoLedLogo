#ifndef AnimationOff_h
#define AnimationOff_h

#include <Animation.h>

class AnimationOff : public Animation {
 public:
  /**
   * Constructor
   */
  AnimationOff(LogoConfig* config, Adafruit_NeoPixel* strip)
      : Animation(config, strip, "Off", "None", "off"){};

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