# MacroPad
A Custom Macro Pad I Designed and Built based on the RP2040

# Features:
* Programmable keys allow shortcuts, macros, or controls to be customized.
* Hotswappable MX switches allows you to customize your typing feel.
* RGB Backlighting allows per-key lighting using WS2812/NeoPixel LEDs
* Custom 3d printed case provides a sleek, ergonomic look.

# Building the MacroPad

**The PCB**
In this repository you can find the raw PCB files and the Gerber files which can allow you to manufacture the PCB through a service such as PCBWay or JLCPCB. I have also attached a BOM containing all the parts for the PCB. Most of the parts utilized on the PCB are very standard, and if you are assembling through JLCPCB the BOM already contains LCSC part numbers for most items. The only items which don't have part numbers, because I did not have them assembled through JLCPCB are the USB Micro-B connector, the reverse mount neopixel LEDs, the MX keyswitch sockets, and a small two pin 2.54mm pitch connector. The USB connector and the two pin connector can be easily bought from many places or even salvaged from old electronics. The [reverse mount LEDs]([url](https://www.adafruit.com/product/4960)) and [MX keyswitch sockets]([url](https://www.adafruit.com/product/4958)) are from adafruit. 

**Switches**
You can use any MX compatible switches with the PCB, but keep in mind that they all have varying heights. The STLs provided are designed for switches with a 7mm above-board height with DSA keycaps, significantly less than most switches. If you want to use a different heigh you will likely have to adjust the STEP file provided to match your keyheight.

**Code**
The code is provided but you will need to adjust it to fit your desires. This includes changing the LED modes and key press actions. Currently, the LED modes includes a constant color mode, an RGB mode, and a layer specific mode. To change layers, you press the first two buttons on the top row and then press the third button. To change colors in the constant color mode, you again press the first two buttons on the top row but then press the fourth button to change colors. To change the actions, scroll down to find the section where they are defined. The code shows how to do shortcuts, go to links, and how to create a shift key for a layer. To upload the code for the first time, short the two pins in the 2.54mm connector and then plug the MacroPad in through USB. It should pop up as detected and then yo ucan unshort the pins. Once you have done this once, you should be able to upload just with USB but if you are having issues uploading you may need to repeat the process.
