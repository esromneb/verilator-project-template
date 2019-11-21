
Assume the hardware is a Lattice iCEstick with 12 MHz clock.
Physical pin choices are not important. Assume "on" or "asserted" is HIGH.
Synthesis will be done with open-source toolchain, icestorm/yosys/etc.

t_min and t_max are constant values of about 2940 and 4320 clock periods.
(About 245 and 360 microseconds respectively, assume t_min < t_max, may be tweaked.)

Input 'A' and input 'B', each one bit. A and B inputs are edge-triggered on positive edge.
'A' will turn on output 'C' and latch it on. Timekeeping starts at this point.
Output 'C', one bit, initially low. C will turn off when t > t_max, always, regardless of the B input.
C will turn off if B is triggered, but does NOT turn off if t < t_min, even if B input is asserted.

The output pulse at C will have a duration that is always >= t_min and always <= t_max.
Depending on the arrival of the B input, the turn-off time may be variable within that window.
When C is turned off, it stays off pending the arrival of another A input.