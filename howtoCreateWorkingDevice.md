# Introduction #

This page is supposed to explain how one should use information, this project offers, to create a real device.

Building an instance of this device is divided into steps.

# Steps #

## Look at how it has been done ##

There exists a working instance of the device, created in year 2009. The device
has been implemented in technics, described as "in solder this, connect with a wire to that, connect again, solder in that small thing, ...., find yourself in a mess of wires".

Documentation has some photos.

However, it works. The only trouble has been screening the sensitive digital part of the device from electromagnetic surges. Without proper protection, there have been problems with:
  * GSM mobile phone, sending data near
  * house refrigerator, starting to operate
  * Aquarium tube lamp, starting itself with a discharge
One shall pay additional attention to screening thermometer chip. An experiment has shown that it causes many instability troubles if not electromagnetically protected well.

## Choose hardware ##
One, willing to construct the device, apart from basic elements (resistors, capacitors,...) would need:
  * ATMega16L microcontroller. Note the letter L - it means low voltage. This μ is able to operate over 2.7..5.5 V diapason.
  * DS1621 thermometer chip.
  * WH0802A-YGH-CT - LCD display. This one's codepage holds Cyrrilics, Latin and some other symbols. I believe, that such device, localised for other languages, would have it's Latin codepage part overlapping with this one.
  * 5 V power adapter, strong enough to give current to relays.
  * 3 electromagnetic relays. The scheme may be, of cause, modified so that to replace electromagnetic (power-inefficient!) relays with optoelectronical switches.
  * A pack of 2N7002 mosfet transistors
  * Some BAT54C Schottky diodes (this comes in pairs in small SMD packs, so you may consider choosing something else)
  * 32768 Hz clock resonator with 3-30 pf variable capacitor
  * 5v self-beeping beeper (beeps on constant voltage)
Everything, that exists on schemas, but is not mentioned here, is considered to be "basic" :P.

## Assemble hardware ##

Doing so manually is a difficult job (I guarantee this!). However, difficulty is relative.

And don't forget: "logical" part of this device needs proper screening.

This project has some datasheets, copy-pasted for around. See:
http://code.google.com/p/aquarium-avr-controller/source/browse/#svn/documentation/datasheets.

### Thermometer connector ###
Read 'physics' wiki page before. It has enough info about the connector.


### LCD connector ###
The wire system, that connects LCD to microcontroller, should be short.

Data transmission rate may be changed by editing number of asm("nop") in display.c : tact()  and  display.c : wait\_lcd\_ready() functions.


## Upload firmware ##
There are many ways to program AVR chips, any can be used. This device schematics, however, is designed for use with "dapa" programmer. Dapa is very simple: some wires with resistors, connected to computer LPT port. Dapa circuit is among the other in project files. If you plan to use another programmer, change programming socket schema so that only microcontroller RESET resistor is left (the problem is that programming socket is combined with a resistor, that pulls RESET up).

Programming involves two steps:
  * Set fuse bytes: Fuse Low Byte = 0xa1, Fuse High Byte = 0xd9
  * upload firmware from file aquarium.hex

In case of using a Linux box with lpt-attached dapa the commands to perform these operations would be:
```
    sudo -u lp uisp -dprog=dapa --wr_fuse_l=161 --wr_fuse_h=217
    sudo -u lp uisp -dprog=dapa --erase
    sudo -u lp uisp -dprog=dapa --upload if=aquarium.hex
```

After uploading firmware the device shall start operating with clock set to 12:00:00. DS1621 thermometer flash internal parameters are checked at startup and fixed, if it is necessary. Upon changing something in user interface, configuration is backed up to EEPROM, so that if microcontroller gets reset (electromagnetic surge, power-off with bad internal back-up battery), only time will need adjusting.