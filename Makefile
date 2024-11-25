.SUFFIXES:
ifeq ($(strip $(DEVKITPPC)),)
$(error "no DEVKITPPC defined. export DEVKITPPC=<path to>devkitPPC")
endif

THUMB=0

include $(DEVKITPPC)/wii_rules

#---------------------------------------------------------------------------------
TARGET      :=  $(notdir $(CURDIR))
BUILD       :=  build
SOURCES     :=  src
ARM         :=  src/arm
DATA        :=
#---------------------------------------------------------------------------------
CFLAGS      += -g -O2 -Wall $(MACHDEP) $(INCLUDE)
CXXFLAGS    += $(CFLAGS)
LDFLAGS     += -g $(MACHDEP) -Wl,-Map,$(notdir $@).map
#---------------------------------------------------------------------------------
LIBS        := -lwiiuse -lbte -logc -lm
#---------------------------------------------------------------------------------
LIBDIRS     :=
#---------------------------------------------------------------------------------
CFILES      :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES    :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES      :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s))) \
                 $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
    LD   :=  $(CC)
else
    LD   :=  $(CXX)
endif

OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
OFILES := $(addprefix $(BUILD)/, $(OFILES_SOURCES))
INCLUDE  :=  -I$(DEVKITPPC)/../powerpc-eabi/include/ \
             -I$(CURDIR)/include \
              $(foreach dir,$(LIBDIRS), -I$(dir)/include) \
              -I$(CURDIR)/$(BUILD) \
              -I$(LIBOGC_INC)
LIBPATHS := -L$(LIBOGC_LIB) $(foreach dir,$(LIBDIRS), -L$(dir)/lib)
#---------------------------------------------------------------------------------
.PHONY: all clean run

all: $(TARGET).dol

$(TARGET).dol: $(TARGET).elf
	$(STRIP) $<
	elf2dol $< $@

$(TARGET).elf: include/arm.h $(OFILES)
	$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

#---------------------------------------------------------------------------------
$(BUILD)/%.o: $(SOURCES)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -MMD -MP -MF $(BUILD)/$*.d -c $< -o $@

$(BUILD)/%.o: $(SOURCES)/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -MMD -MP -MF $(BUILD)/$*.d -c $< -o $@

#$(BUILD)/%.o: $(SOURCES)/%.s | $(BUILD)
#	$(AS) $(ASFLAGS) -I ../bytecode $< -o $@

include/arm.h : $(ARM)/arm.bin
	@echo "\n#include <stdbool.h>" > $@
	@echo "#include <stdint.h>" >> $@
	@echo "static const unsigned int arm_code[] __attribute__((section(\".arm_code\"))) = {" >> $@
	@hexdump -v -e '"0x" 4/1 "%02x" ",\n"' $(ARM)/arm.bin >> $@
	@echo "};" >> $@
	@echo "void run_arm(uint32_t* code, int len, bool thumb);" >> $@
$(ARM)/arm.bin : $(ARM)/arm.elf 
	arm-none-eabi-objcopy -Obinary $< $@
$(ARM)/arm.elf : $(wildcard $(ARM)/*.s) bytecode/bytecode $(ARM)/linker.ld
	@if [ $(THUMB) = 0 ]; then \
		arm-none-eabi-as -march=armv5te -mcpu=arm926ej-s -mbig-endian $(ARM)/arm_code.s -o src/arm/arm.o; \
	else \
		arm-none-eabi-as -mthumb -march=armv5te -mcpu=arm926ej-s -mbig-endian $(ARM)/arm_code.s -o src/arm/arm.o; \
	fi
	arm-none-eabi-ld -T src/arm/linker.ld src/arm/arm.o -o $@
$(BUILD):
	mkdir -p $(BUILD)

-include $(wildcard $(BUILD)/*.d)

clean:
	rm -rf $(BUILD) $(TARGET).elf $(TARGET).dol $(TARGET).elf.map include/arm.h src/arm/arm.elf src/arm/arm.bin src/arm/arm.o
run:
	wiiload $(TARGET).dol
