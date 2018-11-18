#ifndef AnimationFire_h
#define AnimationFire_h

#include <Animation.h>

/**
 * Fire animation.
 */
class AnimationFire : public Animation {
 public:
  /**
   * Constructor
   */
  AnimationFire(Adafruit_NeoPixel* strip)
      : Animation(strip, "Fire", "None", "fire"){};

  /**
   * Performs all update operations of the LEDs when the underlying delay is
   * passed.
   */
  void process() override;

 private:
  void setPixelHeatColor(int Pixel, byte temperature);
};

#endif