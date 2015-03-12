GNU_INSTALL_ROOT = /home/ghost/embedded/arm/gcc-arm-none-eabi-4_9-2014q4/
ASM_EXT = s
OUTNAME = LPC177x_8x_test
LD_SCRIPT = ldscript_rom_gnu.ld
CHKSM = crc
PROJ_ROOT = .
DEVICE_INC_DIR = $(PROJ_ROOT)/Drivers/include



GNUTOOLS		='$(GNU_INSTALL_ROOT)/bin'
THUMB2GNULIB 	='$(GNU_INSTALL_ROOT)/lib/gcc/arm-none-eabi/$(GNU_VERSION)/thumb'
THUMB2GNULIB2	='$(GNU_INSTALL_ROOT)/arm-none-eabi/lib/thumb2'

CPU       		= cortex-m3
OPTIM			= s

#===================== C compile flag ============================
CFLAGS    		= -c 
CFLAGS			+= -mcpu=$(CPU) 
CFLAGS			+= -mthumb 
CFLAGS			+= -Wall 
CFLAGS			+= -O$(OPTIM) 
CFLAGS			+= -mapcs-frame 
CFLAGS			+= -D__thumb2__=1 
CFLAGS	 		+= -msoft-float 
#CFLAGS	 		+= -mfloat-abi=soft 
CFLAGS			+= -gdwarf-2 
CFLAGS   		+= -mno-sched-prolog 
CFLAGS			+= -fno-hosted 
CFLAGS			+= -mtune=cortex-m3  
CFLAGS			+= -march=armv7-m 
CFLAGS			+= -mfix-cortex-m3-ldrd  
CFLAGS   		+= -ffunction-sections 
CFLAGS			+= -fdata-sections 

#================= note =================================
#CFLAGS			+= -mno-bit-align 
#CFLAGS			+= mstructure-size-boundary=8
#CFLAGS			+= -Wpacked
#CFLAGS			+= -Wpadded
#CFLAGS			+= -fpack-struct=0 

#================ note2 ================================
#CFLAGS			+= -fno-builtin
#CFLAGS			+= -fno-strict-aliasing  
#CFLAGS			+= -D PACK_STRUCT_END=__attribute\(\(packed\)\)
#CFLAGS			+= -D ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\)
#CFLAGS			+= -fmessage-length=0 
#CFLAGS			+= -funsigned-char 
#CFLAGS			+= -Wextra 
#CFLAGS			+= -MMD 
#CFLAGS			+= -MP 
#CFLAGS			+= -MF"$(@:%.o=%.d)" 
#CFLAGS			+= -MT"$(@:%.o=%.d)" 

#================ Build Folder Include ========================
CFLAGS                  += $(TOOLDIRINC)
CFLAGS   		+= -I$(FWLIB_INC_DIR)
CFLAGS   		+= -I$(CMCORE_INC_DIR) -I$(DEVICE_INC_DIR) -I$(BOARD_SUPPORT_INC_DIR)
CFLAGS			+= -I$(DEVICE_INC_DIR)/../.. -I./

#================ Asm compile flag ========================
AFLAGS    		= -mcpu=$(CPU) 
AFLAGS   		+= -I$(FWLIB_INC_DIR) -I$(CMCORE_INC_DIR) -I$(DEVICE_INC_DIR)  -I$(BOARD_SUPPORT_INC_DIR) -gdwarf-2 
AFLAGS			+= --defsym RAM_MODE=0


CC		 		= $(GNUTOOLS)/arm-none-eabi-gcc
#CC       		= $(GNUTOOLS)/arm-none-eabi-gcc-$(GNU_VERSION)
AS       		= $(GNUTOOLS)/arm-none-eabi-as
AR       		= $(GNUTOOLS)/arm-none-eabi-ar -r
LD       		= $(GNUTOOLS)/arm-none-eabi-gcc
NM       		= $(GNUTOOLS)/arm-none-eabi-nm
OBJDUMP  		= $(GNUTOOLS)/arm-none-eabi-objdump
OBJCOPY  		= $(GNUTOOLS)/arm-none-eabi-objcopy
READELF  		= $(GNUTOOLS)/arm-none-eabi-readelf
CODESIZE 		= $(GNUTOOLS)/arm-none-eabi-size


#================ LD flag ========================
LDFLAGS  		= -nostartfiles 
LDFLAGS 		+= -nodefaultlibs 
LDFLAGS 		+= -nostdlib 
LDFLAGS			+= -T $(LD_SCRIPT)
LDFLAGS			+= -mcpu=$(CPU)
LDFLAGS	 		+= -msoft-float 
#LDFLAGS	 		+= -mfloat-abi=soft 
LDFLAGS	 		+= -mthumb 


LK       		=  -static -mcpu=cortex-m3 -mthumb
#LK			= -static
#LK       		+= -Wl,--start-group $(TARGET_FWLIB_LIB) 
LK       		+= -Wl,--start-group 
LK 				+= -L$(THUMB2GNULIB) -L$(THUMB2GNULIB2)
#LK       		+= -lc -lg -lstdc++ -lsupc++ 
#LK 				+= -lgcc -lm 
LK 				+= -lm 
LK       		+= -Wl,--end-group
LK				+= -nosys
#LK       		+= -Wl,--gc-sections -Wl,--print-gc-sections 



MAP      		= -Xlinker -Map -Xlinker
LDESC    		= -Xlinker -T  
ENTRY    		= -e
BIN      		= .bin
EXT      		=.elf
LEXT     		= 
REC      		=.srec
HEX		 		=.hex


SRC   	 =$(wildcard *.c) $(wildcard Drivers/source/*.c) $(wildcard fatfs10b/*.c)
SRCC  	 =$(wildcard *.cpp)
ASM	 =$(wildcard *.$(ASM_EXT))
OBJS  =$(SRC:%.c=%.o) $(SRCC:%.c=%.o) $(ASM:%.$(ASM_EXT)=%.o)

bin: elf
	$(OBJCOPY) -O binary $(OUTNAME)$(EXT) $(OUTNAME)$(BIN)
	$(CHKSM) $(OUTNAME)$(BIN)
	$(CODESIZE) $(OUTNAME)$(EXT)

elf: objs
	$(LD) $(LDFLAGS) -o $(OUTNAME)$(EXT) $(OBJS)

%.o : %.s
	@echo AS $<
	$(AS) $(AFLAGS) $< -o $*.o

#objs: .depend $(OBJS)
objs:  $(OBJS)

clean:
	@$(RM) $(OBJS)
	@$(RM) *.o
	@$(RM) *.elf
	@$(RM) *~
	@$(RM) *.map
	@$(RM) *.lst
	@$(RM) *.axf
	@$(RM) *.bin
	@$(RM) *.dbo
	@$(RM) *.dla
	@$(RM) *.dnm
	@$(RM) *.srec
	@$(RM) *.wrn
	@$(RM) *.c.orig
	@$(RM) *.h.orig
	@$(RM) .viafile
	@$(RM) .vias
	@$(RM) *.hex

realclean: clean
	@$(RM) *.depend
