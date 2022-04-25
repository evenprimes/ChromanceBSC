# ChromanceBSC

This is a FastLED version of the amazing
[Chromance](https://github.com/ZackFreedman/Chromance)project by
[Zach Freedman](http://youtube.com/zackfreedman).

I wasn't able to get his code working, and that was apparently with good reason.
;)

This version is based on FastLED and the Wemos D1 lite. I use PlatformIO for VS
Code, so dependencies are hopefully documented clearly in the `platformio.ini`
file.

As of the current version, I find that the patterns change a bit too fast for
the mellow feel I'm going for. I'll have to play around with that when I get a
little time.

## MQTT Control

Listens for 3 MQTT topics:

- ledsculpture/mode - to control the mode
- ledsculpture/pattern - to control the pattern(s)
- ledsculpture/palette - to control the palette

I used MQTT so that I could automate/control this via
[Home Assistant](https://www.home-assistant.io/) and
[Node Red](https://nodered.org/). But as long as you have an MQTT server, you do
you.

### Modes

_Individual_ Value 1

This mode is the "classic" where each strip operates mostly independantly.

When in this mode, the values of pattern and palette are ignored.

_Unified_ Value 2

This mode creates a single matrix upon which different patterns and palettes are
applied. A single palette and pattern are used, though there is a "random"
pattern.

_Random_ Value 3

This mode creates a single matrix upon which different patterns and palettes are
applied. Random pattern and palettes will be selected over time.

When in this mode, the values of pattern and palette are ignored.

### Patterns

Patterns are all mostly self explanatory. The code is 99% ~~stolen~~ borrowed from
[Jason Coon's LED Mapper](https://jasoncoon.github.io/led-mapper/) project. (I needed to alter it slightly to work with mapping the leds to 4 strips.)

- _randomize_ Value 0 - Randomly jumps between the rest of the patterns
- _beatsin8_ Value 1
- _clockwise_ Value 2
- _counterClockwise_ Value 3
- _outward_ Value 4
- _inward_ Value 5
- _north_ Value 6
- _northEast_ Value 7
- _east_ Value 8
- _southEast_ Value 9
- _south_ Value 10
- _southWest_ Value 11
- _west_ Value 12
- _northWest_ Value 13

### Palettes

Palettes are mostly just the built-in palettes.

- _RainbowColors_p_ Value 0
- _RainbowStripeColors_p_ Value 1
- _CloudColors_p_ Value 2
- _LavaColors_p_ Value 3
- _OceanColors_p_ Value 4
- _ForestColors_p_ Value 5
- _PartyColors_p_ Value 6
- _HeatColors_p_ Value 7
- _IceColors_p_ Value 8

## Power

I'm running with a
[5A supply off Amazon](https://smile.amazon.com/gp/product/B078RT3ZPS/). Zack's original project could get by with far less since the LEDs were mostly off. With this setup, the LEDs are usually on, and often at near full brightness.

## Code Style

I'm mostly a Python guy, so I kind of write Python even in C++ right now. Naming
is a bit... off because I did a lot of copy/paste programming. /shrug

*Version 2022.1.0*
