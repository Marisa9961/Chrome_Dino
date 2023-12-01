###################################################
# Generic Makefile (based on gcc)
###################################################

######################################
# target
######################################
TARGET = target

######################################
# building variables
######################################
CONFIGURE_FILE = Configure.mk

include $(CONFIGURE_FILE)

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

# 构建输出
OUTPUT_DIR = target

#######################################
# binaries
#######################################

# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S


######################################
# C sources
######################################
# C sources
C_SOURCES +=  \
	$(wildcard App/Src/*.c) \
	$(wildcard Mcu/Core/Src/*.c) \
	$(wildcard Mcu/Drivers/STM32F1xx_HAL_Driver/Src/*.c) 

# C includes
C_INCLUDES =  \
	-IMcu/Core/Inc \
	-IMcu/Drivers/STM32F1xx_HAL_Driver/Inc \
	-IMcu/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy \
	-IMcu/Drivers/CMSIS/Device/ST/STM32F1xx/Include \
	-IMcu/Drivers/CMSIS/Include \
	-IApp/Inc 
 
#######################################
# CFLAGS
#######################################
# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# # macros for gcc
# # AS defines
AS_DEFS = 

# AS includes
AS_INCLUDES = 

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(C_VER) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(OUTPUT_DIR)/$(TARGET).elf $(OUTPUT_DIR)/$(TARGET).hex $(OUTPUT_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile $(CONFIGURE_FILE) | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile $(CONFIGURE_FILE) | $(BUILD_DIR)
	$(AS) -c $(ASLAGS) $< -o $@

$(OUTPUT_DIR)/$(TARGET).elf: $(OBJECTS) Makefile $(CONFIGURE_FILE) | $(OUTPUT_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(OUTPUT_DIR)/%.hex: $(OUTPUT_DIR)/%.elf | $(OUTPUT_DIR)
	$(HEX) $< $@
	
$(OUTPUT_DIR)/%.bin: $(OUTPUT_DIR)/%.elf | $(OUTPUT_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

$(OUTPUT_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR) $(TARGET)

clear: clean

#######################################
# FLASH(openocd)
#######################################
flash: $(OUTPUT_DIR)/$(TARGET).hex
	$(OPENOCD) $(OPENOCD_CFG) -c "program $(OUTPUT_DIR)/$(TARGET).hex" -c "reset run" -c "exit"

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
