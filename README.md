Runner's GPS
============

Code and design files for my Runner's GPS project
(https://sites.google.com/a/umn.edu/tayl0518-runners-gps/)


A few helpful notes:

-Maps tiles can either be downloaded from the OSM project,
or created yourself (eg, via TileMill). Either way, they'll
have to not only be converted into bitmaps (RGB565), but
they'll ALSO have to be flipped vertically. Bitmaps are
weird and store the last row first, and this code reads the
images as you would read a book. The code could probably
be changed, but if you're flipping images already, why?

-In addition to the license for this overall code, some parts
have their own licensing. The FatFS library is ~BSD-licensed,
and the SD/MMC driver code is BSD-licensed. The NXP LPC17xx
support code (in Drivers/*) seems to be "do whatever you want
with this, but don't sue us". So I guess it's pretty safe to
assume everything is under a BSD license. In any case, the
relevant files have copyright notices in them.

-This code should compile with gcc-arm-embedded, from
https://launchpad.net/gcc-arm-embedded
