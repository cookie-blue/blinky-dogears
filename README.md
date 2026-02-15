# Blinky Dogears

[Blink Patterns](img/blink/README.md)

## Used components

* 5V COB LED Strip 3mm ([AliExpress](https://aliexpress.com/item/1005006461651051.html))
* ESP32-C3 ([Amazon](https://www.amazon.com/dp/B0DD3ZB5XV))
* 36AWG (0.0127 mm²) Wire ([AliExpress](https://aliexpress.com/item/4001028437947.html))
* 2N2222 Transistor ([AliExpress](https://aliexpress.com/item/1005002840840376.html))
* Elastic Band ([Amazon](https://www.amazon.com/dp/B0F59RBNKZ))

## Bluetooth

this version supports controlling via Bluetooth LE for example using the BLE Controller App for [iOS](https://apps.apple.com/app/id6754522781) and [Android](https://play.google.com/store/apps/details?id=com.circuitmagic.iot&hl=en-US&pli=1).

The following Commands are Supported:

* `+` Next Pattern (same as the physical button)
* `-` Previous Pattern
* `PX` Pattern X (`P1`, `P2`, `P3` etc.)
* `BX` Constant brightness X% (gamma corrected) (`B10` = 10%, `B69` = 69% etc.) 
* `OFF` Turn LEDs off
* `ON` Turn LEDs on and resume Pattern

## PCB

> [!WARNING]
> TODO: this is still the old PCB.

![](pcb/pcb_schem.jpg)

## Photos

![](img/IMG01.jpg)

![](img/IMG02.jpg)

![](img/IMG03.jpg)

![](img/IMG04.jpg)

![](img/IMG05.jpg)

> [!NOTE]
> YES THE MOUNTING NEEDS SOME WORK

![](img/IMG06.jpg)

![](img/IMG07.jpg)
