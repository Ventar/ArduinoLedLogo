#ifndef AnimationFadeInOut_h
#define AnimationFadeInOut_h

#include <Animation.h>

/**
 * Mode of the animation, i.e. if the LED brightness is increased or decreased.
 */
enum Mode { UP, DOWN };

/**
 * Fade animation.
 */
class AnimationFadeInOut : public Animation {
 public:
  /**
   * Constructor.
   */
  AnimationFadeInOut(Adafruit_NeoPixel* strip)
      : Animation(strip, FADE_IN_OUT, "Fade In Out", STRIP, "Strip", "fade"){};

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
  Mode mode = UP;
};

#endif