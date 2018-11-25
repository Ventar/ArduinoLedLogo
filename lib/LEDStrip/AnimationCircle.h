#ifndef AnimationCircle_h
#define AnimationCircle_h

#include <Animation.h>

class AnimationCircle : public Animation {
 public:
  /**
   * Constructor
   */
  AnimationCircle(LogoDynamicConfig* config, Adafruit_NeoPixel* strip)
      : Animation(config, strip, "Circle", "Strip", "circle"){};

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