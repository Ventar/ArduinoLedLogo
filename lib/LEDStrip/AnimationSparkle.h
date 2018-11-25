#ifndef AnimationSparkle_h
#define AnimationSparkle_h

#include <Animation.h>

/**
 * Sparkle animation.
 */
class AnimationSparkle : public Animation {
 public:
  /**
   * Constructor.
   */
  AnimationSparkle(LogoDynamicConfig* config, Adafruit_NeoPixel* strip)
      : Animation(config, strip, "Sparkle", "Strip", "sparkle") {
    memory = new uint8_t[45]{0};
  };

  /**
   * Resets the animation to the initial values.
   */
  void reset() override;

  /**
   * Performs all update operations of the LEDs when the underlying delay is
   * passed.
   */
  void process() override;

 private:
  uint8_t* memory;
};

#endif