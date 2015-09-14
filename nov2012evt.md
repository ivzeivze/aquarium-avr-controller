# Introduction #

On november 2012, after several years of stable operation, a nasty fail occured.

# Details #
In the morning, at 0'th minute of some hour the device felt into a bad state. The failure must have been caused by electromagnetic disturbances from switching relays.

In addition, the device could not escape that bad state: watchdog didn't work properly. What was happening was pretty much like if program main() function was being called by real time clock interruption vector: the processes had a period of one second. All external devices were being randomly switched up and down.

Version 0.9.7 has been devoted to fixing this issue. In this version "wdr" instruction has been removed from everywhere except one place in the main loop. In my opinion, the abundancy of "wdr" instruction in code was the reason for watchdog to keep happy, while bad things were actually happening.