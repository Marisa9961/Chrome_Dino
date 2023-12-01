######################################
# building variables
######################################
# debug build?
DEBUG = 1

# optimization 最优化
OPT = -O0

#Flash
OPENOCD = openocd
OPENOCD_CFG = -f openocd.cfg

OPENOCD_FLASHYP_DRIVE = E:/

######################################
# GCC/G++ binary prefix
######################################
PREFIX = arm-none-eabi-

######################################
# C source
######################################
# C_SOURCES =  \
#     $(wildcard Mcu/FATFS/App/*.c) \
#     $(wildcard Mcu/FATFS/Target/*.c) \
#     $(wildcard Mcu/Middlewares/Third_Party/FatFs/src/*.c) \
#     $(wildcard Mcu/Middlewares/Third_Party/FatFs/src/option/*.c)  \
#     $(wildcard Mcu/LIBJPEG/App/*.c) \
#     $(wildcard Mcu/Middlewares/Third_Party/LibJPEG/source/*.c)


# #C_INCLUDES
# C_INCLUDES = \
#     -IMcu/FATFS/App \
#     -IMcu/FATFS/Target \
#     -IMcu/Middlewares/Third_Party/FatFs/src \
#     -IMcu/Middlewares/Third_Party/FatFs/src/option \
#     -IMcu/LIBJPEG/App \
#     -IMcu/LIBJPEG/Target \
#     -IMcu/Middlewares/Third_Party/LibJPEG/include 

# ######################################
# # C++ source
# ######################################
# CPP_SOURCES =

# CPP_INCLUDES =


#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F103xB


#######################################
# ASM sources
#######################################
ASM_SOURCES =  \
	Mcu/startup_stm32f103xb.s

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = Mcu/STM32F103C8Tx_FLASH.ld
