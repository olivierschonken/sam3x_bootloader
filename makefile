#
#       !!!! Do NOT edit this makefile with an editor which replace tabs by spaces !!!!    
#
##############################################################################################
# 
# On command line:
#
# make -f makefile CFG=Debug all = Create Debug project
#
# make -f makefile CFG=Release all = Create Release project
#
# make clean = Clean project files.
#
# To rebuild project do "make clean" and "make all".
#

# If no configuration is specified, "Debug" will be used
ifndef "CFG"
	CFG=Debug
endif

CHIP  = sam3x8
BOARD = sam3x_ek

# Trace level used for compilation
# (can be overriden by adding TRACE_LEVEL=#number to the command-line)
# TRACE_LEVEL_DEBUG      5
# TRACE_LEVEL_INFO       4
# TRACE_LEVEL_WARNING    3
# TRACE_LEVEL_ERROR      2
# TRACE_LEVEL_FATAL      1
# TRACE_LEVEL_NO_TRACE   0
TRACE_LEVEL = 4

##############################################################################################
# Start of default section
#

TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
LD   = $(TRGT)ld
AS   = $(TRGT)gcc -x assembler-with-cpp
BIN  = $(CP) -O binary 

MCU  = cortex-m3

# List all default C defines here, like -D_DEBUG=1
ifeq "$(CFG)" "Debug"
	DDEFS = -DDebug
endif
ifeq "$(CFG)" "Release"
	DDEFS = -DRelease
endif
#-Dpsram
#-DKernelApp
#-DMainApp 
#-Dpsram 

# List all default ASM defines here, like -D_DEBUG=1
DADEFS = 

# List all default directories to look for include files here
DINCDIR = ./inc ./inc/chip ./inc/board ./external_libs ./src/memories/include ./src/memories ./src/fs

# List the default directory to look for the libraries here
DLIBDIR = 

# List all default libraries here
DLIBS = 

#
# End of default section
##############################################################################################

##############################################################################################
# Start of user section
#

# Define project name here
ifeq "$(CFG)" "Debug"
	PROJECT = bootloader_Debug
endif
ifeq "$(CFG)" "Release"
	PROJECT = bootloader_Release
endif

# Define linker script file here
LDSCRIPT_RAM = ./prj/bootloader.ld

# List all user C define here, like -D_DEBUG=1
UDEFS = 

# Define ASM defines here
UADEFS = 

# List C source files here
       
SRC  = ./external_libs/cmsis/core_cm3.c \
	   ./src/main.c \
       ./src/exceptions.c \
	   ./src/linuxboot.c \
       ./src/peripherals/chipid/chipid.c \
       ./src/peripherals/dma/dmac.c \
       ./src/peripherals/eefc/eefc.c \
       ./src/peripherals/hsmc/hsmci.c \
       ./src/peripherals/matrix/matrix.c \
	   ./src/peripherals/pio/pio_it.c \
       ./src/peripherals/pio/pio.c \
       ./src/peripherals/pmc/pmc.c \
       ./src/peripherals/rstc/rstc.c \
       ./src/peripherals/rtc/rtc.c \
       ./src/peripherals/rtt/rtt.c \
       ./src/peripherals/sdram/sdramc.c \
       ./src/peripherals/smc/smc.c \
       ./src/peripherals/spi/spi.c \
       ./src/peripherals/supc/supc.c  \
       ./src/peripherals/twi/twi.c \
       ./src/peripherals/usart/usart.c \
       ./src/peripherals/wdt/wdt.c \
       ./src/startup/board_cstartup_gnu.c \
	   ./src/startup/board_lowlevel.c \
       ./src/startup/board_memories.c \
	   ./src/drivers/led/led.c \
	   ./src/drivers/syscalls.c \
	   ./src/drivers/trace.c \
	   ./src/drivers/uart_console.c \
	   ./src/drivers/lcd_draw.c \
	   ./src/drivers/lcd_font.c \
	   ./src/drivers/lcd_font10x14.c \
	   ./src/drivers/lcdd.c \
	   ./src/drivers/hx8347.c \
	   ./src/drivers/frame_buffer.c \
	   ./src/drivers/dmacd.c \
	   ./src/memories/nandflash/EccNandFlash.c \
       ./src/memories/nandflash/ManagedNandFlash.c \
       ./src/memories/nandflash/MappedNandFlash.c \
       ./src/memories/nandflash/NandFlashModel.c \
       ./src/memories/nandflash/NandFlashModelList.c \
       ./src/memories/nandflash/NandSpareScheme.c \
       ./src/memories/nandflash/TranslatedNandFlash.c \
       ./src/memories/nandflash/NfcRawNandFlash.c \
       ./src/memories/Media_Init.c \
       ./src/memories/Media.c \
       ./src/memories/MEDNandFlash.c \
       ./src/fs/ff.c \
	   ./src/fs/diskio.c \
	   ./src/memories/sdmmc/mci_cmd.c \
	   ./src/memories/sdmmc/sdmmc.c \
	   ./src/memories/MEDSdcard.c 
	                
# List ASM source files here
ASRC =

# List all user directories here
UINCDIR = ./inc

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS = 

# Define optimisation level here
OPT = -O0

#
# End of user defines
##############################################################################################


INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))
DEFS    = $(DDEFS) $(UDEFS)
ADEFS   = $(DADEFS) $(UADEFS)
OBJS    = $(ASRC:.s=.o) $(SRC:.c=.o)
ifeq "$(CFG)" "Debug"
	OUTDIR  = objects
	OBJOUT  = ./$(OUTDIR)
	LISTDIR = lst
	LISTOUT = ./$(LISTDIR)
endif
ifeq "$(CFG)" "Release"
	OUTDIR  = objects
	OBJOUT  = ./$(OUTDIR)
	LISTDIR = lst
	LISTOUT = ./$(LISTDIR)
	RELEASE = release
endif

LIBS    = $(DLIBS) $(ULIBS)
MCFLAGS = -mcpu=$(MCU)
ifeq "$(CFG)" "Debug"
	ASFLAGS = $(MCFLAGS) -g -gdwarf-2 -Wa,-amhls=$(LISTOUT)/$(*F).lst $(ADEFS)
endif
ifeq "$(CFG)" "Release"
	ASFLAGS = $(MCFLAGS) -Wa,-amhls=$(LISTOUT)/$(*F).lst $(ADEFS)
endif
#CPFLAGS = $(MCFLAGS) $(OPT) -gdwarf-2 -mthumb -fomit-frame-pointer -Wall -Wstrict-prototypes -mfix-cortex-m3-ldrd -fverbose-asm -Wa,-ahlms=$(LISTOUT)/$(*F)lst $(DEFS)
	CPFLAGS += -Wall -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int
	CPFLAGS += -Werror-implicit-function-declaration -Wmain -Wparentheses
	CPFLAGS += -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused
	CPFLAGS += -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef
	CPFLAGS += -Wshadow -Wpointer-arith -Wbad-function-cast -Wwrite-strings
	CPFLAGS += -Wsign-compare -Waggregate-return -Wstrict-prototypes
	CPFLAGS += -Wmissing-prototypes -Wmissing-declarations
	CPFLAGS += -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations
	CPFLAGS += -Wpacked -Wredundant-decls -Wnested-externs -Winline -Wlong-long
	CPFLAGS += -Wunreachable-code
	CPFLAGS += -Wcast-align
	#CPFLAGS += -Wmissing-noreturn
#CPFLAGS += -Wconversion

# To reduce application size use only integer printf function.
#CPFLAGS += -Dprintf=iprintf

# -mlong-calls  -Wall
CPFLAGS += --param max-inline-insns-single=500 -Wa,-amhls=$(LISTOUT)/$(*F).lst $(ADEFS) -fomit-frame-pointer -mthumb -ffunction-sections -mfix-cortex-m3-ldrd
CPFLAGS += -g $(MCFLAGS) $(OPT) -D$(CHIP) -DTRACE_LEVEL=$(TRACE_LEVEL)
ifeq "$(CFG)" "Debug"
CPFLAGS += -gdwarf-2  
endif
	
ifeq "$(CFG)" "Debug"
LDFLAGS_RAM = $(MCFLAGS) -mthumb -nostartfiles -T$(LDSCRIPT_RAM) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch $(LIBDIR)
endif
ifeq "$(CFG)" "Release"
LDFLAGS_RAM = $(MCFLAGS) -mthumb -nostartfiles -T$(LDSCRIPT_RAM) -Wl,-Map=$(PROJECT).map,--cref,--strip-debug,--no-warn-mismatch $(LIBDIR)
endif
# Generate dependency information
CPFLAGS += -MD -MP -MF .dep/$(@F).d

#
# makefile rules
#

all: RAM
ifeq "$(CFG)" "Debug"
makdir:	
	mkdir $(LISTDIR)
	mkdir .dep
	mkdir $(OUTDIR)
endif

ifeq "$(CFG)" "Release"
makdir:	
	mkdir $(LISTDIR)
	mkdir .dep
	mkdir $(OUTDIR)
	mkdir $(RELEASE)
endif

ifeq "$(CFG)" "Debug"	
RAM: makdir $(OBJS) $(PROJECT).elf 
endif

ifeq "$(CFG)" "Release"	
RAM: makdir $(OBJS) $(PROJECT).elf $(PROJECT).bin
endif

%o : %c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) $< -o "$(OBJOUT)/$(@F)"

%o : %s
	$(AS) -c $(ASFLAGS) $< -o "$(OBJOUT)/$(@F)"

%.elf: $(OBJOUT)/*
	$(CC) $(OBJOUT)/* $(LDFLAGS_RAM) $(LIBS) -o $(@F)

%bin: %elf
	$(BIN) $< "$(RELEASE)/$(@F)"

ifeq "$(CFG)" "Release"
clean:
	-cs-rm -f $(OBJS)
	-cs-rm -f $(PROJECT).elf
	-cs-rm -f $(PROJECT).map
	-cs-rm -f $(RELEASE)/$(PROJECT).bin
	-cs-rm -f $(SRC:.c=.c.bak)
	-cs-rm -f $(SRC:.c=.lst)
	-cs-rm -f $(ASRC:.s=.s.bak)
	-cs-rm -f $(ASRC:.s=.lst)
	-cs-rm -fR .dep
	-cs-rm -fR $(OUTDIR)
	-cs-rm -fR $(LISTDIR)
	-cs-rm -fR $(RELEASE)
endif

ifeq "$(CFG)" "Debug"
clean:
	-cs-rm -f $(OBJS)
	-cs-rm -f $(PROJECT).elf
	-cs-rm -f $(PROJECT).map
	-cs-rm -f $(SRC:.c=.c.bak)
	-cs-rm -f $(SRC:.c=.lst)
	-cs-rm -f $(ASRC:.s=.s.bak)
	-cs-rm -f $(ASRC:.s=.lst)
	-cs-rm -fR .dep
	-cs-rm -fR $(OUTDIR)
	-cs-rm -fR $(LISTDIR)
endif


# 
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***