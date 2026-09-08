<!--
source: https://developer.repebble.com/guides/tools-and-resources/hardware-information/
fetched: 2026-09-07T17:42:21Z
-->

[pebble](/)

- [Tutorials](/tutorials/)
- [Get the SDK](/sdk/)
- [Guides](/guides/)
- [Documentation](/docs/)
- [Examples](/examples/)
- [Community](/community/)
- [Blog](https://repebble.com/blog)
- [More](/more/)

[Privacy](https://repebble.com/privacy/)\
[Cookies](https://repebble.com/terms)

[Publish](https://developer.rePebble.com/dashboard)

### [Guides](/guides/)

- [Table of Contents](/guides/toc/)
- [Alloy](/guides/alloy/)
- [App Resources](/guides/app-resources/)
- [Best Practices](/guides/best-practices/)
- [Communication](/guides/communication/)
- [Debugging](/guides/debugging/)
- [Design and Interaction](/guides/design-and-interaction/)
- [Events and Services](/guides/events-and-services/)
- [Graphics and Animations](/guides/graphics-and-animations/)
- [Pebble Packages](/guides/pebble-packages/)
- [Pebble Timeline](/guides/pebble-timeline/)
- [Tools and Resources](/guides/tools-and-resources/)
  - [App Metadata](/guides/tools-and-resources/app-metadata/)
  - [Color Picker Tool](/guides/tools-and-resources/color-picker/)
  - [Command Line Tool](/guides/tools-and-resources/pebble-tool/)
  - [Developer Connection](/guides/tools-and-resources/developer-connection/)
  - [Getting Started with Pebble Time Round](/guides/getting-started-pebble-time-round/)
  - [Hardware Information](/guides/tools-and-resources/hardware-information/)
  - [Internationalization](/guides/tools-and-resources/internationalization/)
- [User Interfaces](/guides/user-interfaces/)

[](javascript:void(0);)

# Hardware Information

The Pebble watch family comprises of multiple generations of hardware, each with unique sets of features and capabilities. Developers wishing to reach the maximum number of users will want to account for these differences when developing their apps.

The table below details the differences between hardware platforms:

Company

Pebble Technology Corporation

Core Devices

Model

Classic,\
Steel

Time,\
Time Steel

Time Round

Pebble 2

Pebble 2 Duo

Pebble Time 2\*

Pebble Round 2

Platform

Aplite

Basalt

Chalk

Diorite

Flint

Emery

Gabbro

SOC

[STMicro STM32F205RE](https://www.st.com/en/microcontrollers-microprocessors/stm32f205re.html)

[STMicro STM32F411](https://www.st.com/en/microcontrollers-microprocessors/stm32f411.html)

[Nordic nRF52840](https://www.nordicsemi.com/Products/nRF52840)

[SiFli SF32LB52J](https://www.sifli.com/en/node/31)

CPU

Cortex-M3\
64 MHz

Cortex-M4\
100 MHz

Cortex-M4\
64 MHz

[Star-MC1](https://www.armchina.com/mountain?infoId=160&name=%22%E6%98%9F%E8%BE%B0%22%20STAR-MC1&CategoryStr=%22%E6%98%9F%E8%BE%B0%22&pStatus=)\
(Cortex-M33-like)\
240 MHz

Max Resource Size

96k

256k

Max App Size (code + heap)

24k

64k

128k

Display Shape

Rectangle

Round

Rectangle

Round

Display Size

1.26"

1.25"

1"

1.26"

1.5"

1.3"

Display Resolution

144 x 168

180 x 180

144 x 168

200 x 228

260 x 260

Display PPI

175

182

175

202

200

Display Colors

2\
*(B/W)*

64\
*(Color)*

2\
*(B/W)*

64\
*(Color)*

Display Manufacturer

Sharp\
*(LS013B7DH05)*

JDI\
*(Color)*

JDI\
*(Color)*

Sharp\
*(LS013B7DH05)*

JDI\
*(LPM015M135A)*

Sharp\
*(LS013B7DD02)*

Touch Screen

No

Yes

Backlight

White LED

Multicolor\
RGB LED

White LED

Heart Rate Monitor

No

No\
*(possible with smartstrap)*

Yes\
*(except SE model)*

No

Yes

No

Microphone

No

Yes

Yes\
*(with 2nd Mic for ENC^(\*1))*

Speaker

No

Yes

No

Sensors

Accelerometer, Compass

Accelerometer

6-axis IMU, Compass, Barometer

6-axis IMU, Compass

3-axis IMU, Compass

Buttons

4

Vibration

ERM (eccentric rotating mass)

LRA (linear resonant actuator)

Case Material

Polycarbonate *(P, PT)*\
Stainless Steel 316 *(PS, PTS)*

Stainless Steel 316

Polycarbonate

Stainless Steel 316

Lens Material

Polycarbonate

Gorilla Glass\
*(curved)*

Gorilla Glass\
*(flat)*

Hardened Glass\
*(flat)*

Hardened Glass\
*(curved)*

Charging Port

Power only

Smart accessory port

Smart accessory port\
*(data only)*

Power only

Battery Life^(\*4)

~7 days

~7 days (Pebble Time), ~10 days (Pebble Time Steel)

~2 days

~7 days

~30 days

~14 days

Water Resistance^(\*3)

5 ATM^(\*2)\
*(50 m)*

3 ATM^(\*2)\
*(30 m)*

None\
*(Splash Resistant)*

3 ATM^(\*2)\
*(30 m)*

20 m\

TBD\
*(target IPX8)*

TBD\
*(target 30m)*

Model

Classic,\
Steel

Time,\
Time Steel

Time Round

Pebble 2

Pebble 2 Duo

Pebble Time 2\*

Pebble Round 2

^(\*1) ENC = Environmental Noise Cancellation

^(\*2) Please note regarding water resistance:

- 3 ATM → Safe for hand-washing, rain, splashes, and brief immersion, but not for swimming/diving.
- 5 ATM → Safe for swimming in shallow water, showering, and snorkeling, but not for diving.
  \

^(\*3)Rated waterproofing is applicable to new, sealed hardware. Age or repairs may compromise water resistance.

^(\*4)Rated battery life is applicable to new hardware in typical usage conditions. Heavy use or use of power intensive apps and watchfaces may lead to a lower battery life. Battery life will reduce as the watch ages. Aftermarket replacement batteries may have a lower capacity.

See [*Available Defines and Macros*](/guides/best-practices/building-for-every-pebble/#available-defines-and-macros) for a complete list of compile-time defines available.

You need JavaScript enabled to read and post comments.
