# ATmega328_PetitFatFs

This is Chang's Petit FatFs module for ATmega328P microcontroller. The library was downloaded from [Chan's website](http://elm-chan.org/fsw/ff/00index_p.html).

pff.c and pff.h were left unmodified.
pffconf.h is were you configure the module.
integer.h was slightly modified, same changes as [cliff and JS did on their example on avrfreaks.com](https://www.avrfreaks.net/forum/tut-c-getting-sdmmc-card-working-painlessly-fatfs?page=all). Just a few extra types added.
disk.h and disk.c were taken from cliff and JS from avrfreak.com, but were slightly modified to work with the ATmega328P.
AK_spi.h and AK_spi.c I wrote myself and they are just the basic functions for SPI.

If you want to use this library for a different ATmega microcontroller, you will have to do modify disk and AK_spi files and configure the pffconf file.

