#ifndef AnimationDoubleCircle_h
#define AnimationDoubleCircle_h

#include <Animation.h>

class AnimationDoubleCircle : public Animation {
 public:
  /**
   * Constructor
   */
  AnimationDoubleCircle(Adafruit_NeoPixel* strip)
      : Animation(strip, "Double Circle", "Strip", "double circle"){};

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