MoodCube
========

What's This?
------------

With mood39 aka Starterkit from [Netz39](http://www.netz39.de/) and the
[infinity mirror cube](http://www.netz39.de/projekte/infinitiy-mirror-cube/) 
from Frank in mind I designed a mood lamp with five RGB LEDs, three
potentiometers and an ATtiny 24/44 controller.

Circuits
--------

Eagle files for both types of PCB are included. One is the base with the
microcontroller and the potentiometers. VCC is intended to be 5V,
however in the first prototype there's no voltage control circuit on
this pcb, so you have to connect a stabilized power supply.

The other one is a PCB for the LED. Current is hold constant by a simple
constant current circuit per color channel. The connection is simply one
cable per channel plus VCC and GND. uC I/O ports are directly connected
to the color lines.

Housing
-------

The housing in the prototype was 6mm plexiglass milled on a CNC machine.
Holding it together was forgotten in design, but the OpenSCAD files are
added here anyway. Simply glueing them is one solution. You could also
make a base plate or anything else.

LED-PCB Mount
-------------

For the prototype I used a frame construction printed with a 3D printer
and lots of hot melt glue. However this was far from optimal and should
not repeated! Please think up a better solution and let me know!

Part List
---------
/tbd/
