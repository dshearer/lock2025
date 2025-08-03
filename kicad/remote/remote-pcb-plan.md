# ATmega328P Key Fob Schematic

## Power Supply
```
CR2032+ ————————————————————————— VCC (Pin 7)
        |                         AVCC (Pin 20)
        └—[100nF]—————————————————— GND (Pin 8, 22)
CR2032- ————————————————————————— GND (Pin 8, 22)
```

## Crystal Oscillator (8MHz Ceramic Resonator)
```
XTAL1 (Pin 9) ————————— CSTCE8M0V53-R0 Pin 1
XTAL2 (Pin 10) ———————— CSTCE8M0V53-R0 Pin 3
GND            ———————— CSTCE8M0V53-R0 Pin 2

Note: No external load capacitors needed - built into resonator
```

## ISP Programming Header
```
MISO (Pin 18) ————————— ISP Pin 1
VCC           ————————— ISP Pin 2  
SCK (Pin 19)  ————————— ISP Pin 3
MOSI (Pin 17) ————————— ISP Pin 4
RESET (Pin 1) ————————— ISP Pin 5
GND           ————————— ISP Pin 6
```

## Reset Circuit
```
VCC ————[10kΩ]———— RESET (Pin 1)
```

## RFM69HCW 915MHz Radio Module
```
VCC ————[100nF ceramic]————— RFM69 VCC (3.3V)
    |
    └——[10µF ceramic/tantalum]— RFM69 VCC (3.3V)
    |
GND ——————————————————————— RFM69 GND  
PB2 (Pin 16) ————————————— RFM69 NSS (SPI Chip Select)
PB3 (Pin 17) ————————————— RFM69 MOSI (SPI Data Out)
PB4 (Pin 18) ————————————— RFM69 MISO (SPI Data In)
PB5 (Pin 19) ————————————— RFM69 SCK (SPI Clock)
PD2 (Pin 4) —————————————— RFM69 DIO0 (Interrupt)
                           RFM69 ANT ——— 8.2cm wire antenna (915MHz)

Decoupling Capacitors:
- 100nF ceramic capacitor (0603/0805) - closest to RFM69 VCC pin
- 10µF ceramic or tantalum capacitor (0805+) - for bulk decoupling
- Place both capacitors as close as possible to RFM69 power pins

## Button Inputs (Case Buttons)
```
Button 1:  PD3 (Pin 5) ——[Pad A]   [Pad B]—— GND
Button 2:  PD4 (Pin 6) ——[Pad C]   [Pad D]—— GND  
Button 3:  PD5 (Pin 11) —[Pad E]   [Pad F]—— GND
Button 4:  PD6 (Pin 12) —[Pad G]   [Pad H]—— GND

Note: Enable internal pull-ups in software for button pins
```

## Pin Assignments Summary

| Function | ATmega328P Pin | Pin Name |
|----------|----------------|----------|
| Power | 7, 20 | VCC, AVCC |
| Ground | 8, 22 | GND |
| Crystal | 9, 10 | XTAL1, XTAL2 |
| Reset | 1 | RESET |
| ISP MISO | 18 | PB4/MISO |
| ISP SCK | 19 | PB5/SCK |
| ISP MOSI | 17 | PB3/MOSI |
| RFM69 NSS | 16 | PB2/SS |
| RFM69 DIO0 | 4 | PD2 |
| Button 1 | 5 | PD3 |
| Button 2 | 6 | PD4 |
| Button 3 | 11 | PD5 |
| Button 4 | 12 | PD6 |

## Notes

- **Much simpler power supply** - only 4 power connections vs 8 for ATmega32U4
- **AVCC** can be tied to VCC for this application
- **Internal pull-ups** used for buttons (no external resistors needed)
- **8MHz operation** - lower power consumption, better for coin cell operation
- **RFM69 low power mode** - use +10dBm to +13dBm transmit power for coin cell compatibility
- **Sleep current** ~0.1µA provides 1-2 year battery life
- **Packet radio features** - addressing, acknowledgments, encryption available

## Arduino Programming

- Select **"Arduino Uno"** board in IDE, then **Tools > Processor > ATmega328P (3.3V, 8MHz)**
- Use **"Arduino as ISP"** or **USBasp** programmer
- Set fuses for **8MHz external crystal**

## Notes

- **ATmega328PB-MUR**: QFN-32 package (5mm x 5mm) for compact design
- **Much simpler power supply** - only 4 power connections
- **AVCC** can be tied to VCC for this application
- **Internal pull-ups** used for buttons (no external resistors needed)
- **8MHz operation** - lower power consumption, better for coin cell operation
- **RFM69 low power mode** - use +10dBm to +13dBm transmit power for coin cell compatibility
- **Sleep current** ~0.1µA provides 1-2 year battery life
- **Packet radio features** - addressing, acknowledgments, encryption available
- **Brown-out detection** recommended for reliable coin cell operation

## References

- **ATmega328P Datasheet**: https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf
- **Arduino ATmega328P Breadboard Guide**: https://docs.arduino.cc/built-in-examples/arduino-isp/ArduinoToBreadboard
- **SparkFun RFM69HCW Breakout**: https://www.digikey.com/en/products/detail/sparkfun-electronics/13909/6023504
- **Polycase FB-45 Mechanical Drawings**: Available from Polycase.com
- **CSTCE8M0V53-R0 Datasheet**: Murata ceramic resonator specificationsf