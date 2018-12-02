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
  AnimationRainbow(LogoConfig* config, Adafruit_NeoPixel* strip)
      : Animation(config, strip, "Rainbow", "None", "rainbow"){};

  /**
   * Performs all update operations of the LEDs when the underlying delay is
   * passed.
   */
  void process() override;

 private:
  uint8_t* wheel(uint8_t WheelPos);
};

#endif