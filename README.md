sam3x_bootloader
================
A Rough bootloader for the SAM3X

This bootloader will search for 2 files on an SD card inserted in the SAM3X development kit.  
1. Kernel Image - /Image
2. Ramdisk Image - /ramdisk

To do a release build (Dependent on an installed Codesourcery arm-none-eabi compiler)

1. make clean
2. make CFG=Release

To do a debug build
1. make clean
2. make CFG=Debug
