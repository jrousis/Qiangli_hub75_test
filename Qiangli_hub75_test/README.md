# Qiangli HUB75 48x24 LED Panel Driver for ESP32

Custom pixel mapping driver for **Qiangli 48x24 HUB75** LED matrix panels using ESP32 and DMA.

## Overview

These panels use a non-standard internal pixel layout. This project provides a `CustomPxBasePanel` class that remaps logical coordinates to the panel's physical wiring, so you can use standard drawing functions (text, pixels, shapes) normally.

## Hardware

| Component | Details |
|---|---|
| MCU | ESP32 |
| Panel | Qiangli 48x24 HUB75 |
| Line Decoder | SM5368 |
| Power | 5V DC |

## Dependencies

- [ESP32 Arduino Core](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)
- [ESP32-HUB75-MatrixPanel-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA) by mrfaptastic

## Configuration

In `Qiangli_hub75_test.ino`:

### Brightness

### Custom Pins

Uncomment the `_pins` parameter in the `HUB75_I2S_CFG` constructor to use custom GPIO pins.

## How the Pixel Mapping Works

The DMA driver is initialized with **doubled width** (96) and **halved height** (12) to match the panel's physical scan pattern. The `CustomPxBasePanel::getCoords()` method then transforms logical (x, y) coordinates to the correct physical positions based on the panel's 6-row interleaved wiring.

## Usage

All [Adafruit GFX](https://learn.adafruit.com/adafruit-gfx-graphics-library) drawing functions are supported.

## Troubleshooting

| Problem | Fix |
|---|---|
| Pixels shifted one column | Toggle `mxconfig.clkphase` between `true`/`false` |
| Memory allocation failed | Check ESP32 free memory or reduce panel count |
| Panel dark | Verify 5V power and HUB75 ribbon cable |

## License

MIT