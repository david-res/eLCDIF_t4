
# eLCDIF_t4

This library is experimental, and is dedicated for use with the Dev board v4 found on PJRC forum - <https://forum.pjrc.com/index.php?threads/call-to-arms-teensy-sdram-true.73898/#post-334006>

The purpose of this library is to display image data on an RGB display using the built in LCD controller of the IMXRT1062 chip by NXP - the eLCDIF controller.

Information about this peripheral can be found in the reference manual, chapter 35.

In order to display data on screens larger than 320*240px @ 16 or 18 bit color depth, external SDRAM is needed (which is only available on the dev board/custom board) and requires the experimental SDRAM_t4 library (<https://github.com/mjs513/SDRAM_t4>).

![a 800*480px 24-bit bus LCD  displaying a static image @ 16.7M colors (24-bit color depth)](/misc/IMG_5146.jpg)

## Usage:

Include the library using:
``` cpp
#include "eLCDIF_t4.h"
```

Create an lcd object using the eLCDIF_t4 class
``` cpp
eLCDIF_t4 lcd;
```

Create a config structure using the lcdif_rgb_mode_config struct

``` cpp
eLCDIF_t4_config config;
```

Call the begin function passing though the following arguments:
* Bus width with the following options: `BUS_8BIT`, `BUS_16BIT`, `BUS_18BIT`, `BUS_24BIT`
* Word Length (color depth) with the following options: `WORD_8BIT`, `WORD_16BIT,` `WORD_18BIT`, `WORD_24BIT`
* Config - referance your config contructor into the begin function
``` cpp
begin(BUS_24BIT, WORD_24BIT, config);
```
