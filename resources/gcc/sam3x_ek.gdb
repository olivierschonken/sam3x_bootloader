echo This config file was tested with J-Link GDB Server v4.10i
echo connect to the J-Link gdb server
echo Setting up the environment for debugging with gdb.\n
set prompt SAM3X JTAG Debug Initialisation
# Step1: Connect to the J-Link gdb server
target remote localhost:2331
mon soft_reset_halt
mon delay 100
mon halt
echo Set JTAG speed to 500kHz
mon jtag_khz 500
echo Set gdb server to little endian
set ENDIAN little
# Step2: Reset peripheral(RSTC_CR) and start UART clock
mon mww 0x400e1A00 0xA5000004
echo Open EBI clock
mon mww 0x400E0610 0x00000100
# Step3: Load file(eg. getting-started project)
load LCD_Example-Debug.elf
# Step4: Initializing stack pointer
mon reg sp 0x20087FFC
break ResetException
break main
continue
