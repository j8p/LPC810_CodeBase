#
# gcc Makefile for LPC810
# Kamal Mostafa <kamal@whence.com>
#
# License: Public Domain
#

CROSS = arm-none-eabi-
CPU = -mthumb -mcpu=cortex-m0plus -MD
FPU = # -mfloat-abi=soft

FILENAME = tlob_iron
LINKERSCRIPT = tlob_iron.ld

CFLAGS+= -Os --specs=nano.specs -ffunction-sections -fdata-sections -fno-builtin

CC = ${CROSS}gcc
LD = ${CROSS}ld
OBJCOPY = ${CROSS}objcopy
SIZE = ${CROSS}size

# use these on the $(LD) line, as needed:
LIBM = ${shell ${CC} ${CFLAGS} --print-file-name=libm.a}
LIBC = ${shell ${CC} ${CFLAGS} --print-file-name=libc.a}
LIBGCC = ${shell ${CC} ${CFLAGS} --print-libgcc-file-name}

CFLAGS += $(CPU) $(FPU)
LDFLAGS += --gc-sections


INC += -I../cmsis
CFLAGS += $(INC) -D__USE_CMSIS # -DCLOCK_SETUP=1

TLOB_IRON_OBJS = main.o \
              system_LPC8xx.o \
              gcc_startup_lpc8xx.o

all: $(FILENAME).bin

tlob_iron.axf: $(FILENAME).ld $(TLOB_IRON_OBJS)
	@$(LD) -o $@ $(LDFLAGS) -T $(LINKERSCRIPT) $(TLOB_IRON_OBJS) $(LIBGCC)
	-@echo ""
	@$(SIZE) $(FILENAME).axf

clean:
	@rm -f *.o *.d
	@rm -f $(FILENAME).axf $(FILENAME).hex $(FILENAME).bin

%.bin:%.axf
	-@echo ""
	-@echo "Generating $(FILENAME).hex (lpc2lisp, FlashMagic, etc.)"
	@$(OBJCOPY) --strip-unneeded -O ihex $(FILENAME).axf $(FILENAME).hex
	-@echo "Generating $(FILENAME).bin"
	@$(OBJCOPY) --strip-unneeded -O binary $(FILENAME).axf $(FILENAME).bin
