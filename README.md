
# MKS bLoAtEd bOoTlOaDeR by darkspr1te

because full fat, why not.

## Bloated Bootloader Version

This version has added lcd init routines for the mks8558 lcd,

the routines will work with other lcd's by changing -DHX8558 on build flags,

check sources for supported models or add your own,

it's getting close to max of the bootloaders file size without optimization

  

We currently load fonts from spi but I intend to hard code fonts as option too

further gfx should be found from spi/i2c prom/sdcard and not stored in loader

cant get too fat now can we ;-)

  

touch might be option too if i can can ,

  

# Repo status and build

This repo may be broken at times and is intended to test out tft ideas in a bootloader

mainly aimed at loading spi/i2c prom update data and in future update itself & via

ESP3d wifi firmwares.

Project may branch into slmmed version for more specific purpose later.

  

Initial commit builds/flashes and works with basic tft /no fonts yet

spi crashes :-/

  
  
  
  

## Getting Started

load into platformio and build - write compiled .vio/build/mkstft28.bin

to 0x8000000 or 0x0 (both same) or use ctrl+alt+u to upload via debugger automagicly

### Deployment

  

Most configuration is in boot_conf.h

uses spi1 and uart1 by default hard coded into main.c/spisd_diskio.c etc

  

add -DDEBUG to build+flags in platformio.ini for usart /debug info,

default usart for MKS is TXD/RXD on wifi port of mkstft28/32 models


  
  

## With Thanks to

  
https://github.com/delwinbest

https://github.com/robotsrulz/ - source for spisd_diskio patches,

  

?? Roman Stepanov - original source for bootloader ??

  ## Notes
  no real central config file yet, many settings still hard coded testing
  

## License

  

A lot of copy pasta code here - various licenses apply

  

## Acknowledgments

  

* Hat tip to anyone whose code was used

* Inspiration

* etc