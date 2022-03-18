# ChromanceBSC

This is a FastLED version of the
(Chromance)[https://github.com/ZackFreedman/Chromance] project by (Zach
Freedman)[http://youtube.com/zackfreedman].

I wasn't able to get his code working, and that was apparently with good reason.
;)

This version is based on FastLED and the Wemos D1 lite. I use PlatformIO for VS
Code, so dependencies are hopefully documented clearly in the `platformio.ini`
file.

Right now this simply does a pretty shifting rainbow pattern, but I hope to make
it a bit nicer over time. (You know, in all the time that makers choose to go
back to a project largely in the "done" category.)

## Power

For the RainbowChase patterns, a 1A power supply seems to be enough when showing
the LEDs at full brightness. For patterns with more of the LEDs on more of the
time, a larger power supply will be needed. I'm running with a (5A supply off
Amazon)[https://smile.amazon.com/gp/product/B078RT3ZPS/].

## Code Style

I'm mostly a Python guy, so I kind of write Python even in C++ right now.
