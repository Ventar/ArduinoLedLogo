#ifndef AnimationCloud_h
#define AnimationCloud_h

#include <Animation.h>

class AnimationCloud : public Animation {
 public:
  /**
   * Constructor
   */
  AnimationCloud(Adafruit_NeoPixel* strip)
      : Animation(strip, "Cloud", "Strip", "cloud"){};

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