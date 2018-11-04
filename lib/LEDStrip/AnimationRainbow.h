#ifndef AnimationRainbow_h
#define AnimationRainbow_h

#include <Animation.h>

/**
 * Rainbow animation.
 */
class AnimationRainbow : public Animation {
 public:
  /**
   * Constructor.
   */
  AnimationRainbow(Adafruit_NeoPixel* strip)
      : Animation(strip, RAINBOW, "Rainbow", NONE, "None", "rainbow"){};

  /**
   * Performs all update operations of the LEDs when the underlying delay is
   * passed.
   */
  void process() override;

 private:
  uint8_t* wheel(uint8_t WheelPos);
};

#endif