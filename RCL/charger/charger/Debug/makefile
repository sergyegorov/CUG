################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

EMPTY := 
SPACE := $(EMPTY) $(EMPTY)
CG_TOOL_ROOT := C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000

ORDERED_OBJS += \
$(GEN_CMDS_QUOTED) \
"D:/work/CUG/V2/charger/DSP2802x_headers/cmd/DSP2802x_Headers_nonBIOS.cmd" \
"D:/work/CUG/V2/charger/Debug/main.obj" \
"D:/work/CUG/V2/charger/Debug/adc.obj" \
"D:/work/CUG/V2/charger/Debug/SPKey.obj" \
"D:/work/CUG/V2/charger/Debug/DSP2802x_GlobalVariableDefs.obj" \
-l"rts2800_ml.lib" \
"D:/work/CUG/V2/charger/DSP2802x_common/cmd/F28023.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include DSP2802x_headers/cmd/subdir_vars.mk
-include DSP2802x_common/cmd/subdir_vars.mk
-include subdir_rules.mk
-include DSP2802x_headers/cmd/subdir_rules.mk
-include DSP2802x_common/cmd/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: D:/work/CUG/V2/charger/Debug/charger.out

# Tool invocations
D:/work/CUG/V2/charger/Debug/charger.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: Linker'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g --define="_DEBUG" --define="LARGE_MODEL" --diag_warning=225 --large_memory_model --obj_directory="D:/work/CUG/V2/charger/Debug" -z -m"D:/work/CUG/V2/charger/Debug/charger.map" --stack_size=300 --warn_sections -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/lib" -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/include" -i"D:/work/CUG/V2/charger/charger" -i"D:/work/CUG/V2/charger" -i"C:/CCStudio_v3.1/C2000/cgtools/lib" --reread_libs --rom_model -o "D:/work/CUG/V2/charger/Debug/charger.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C2000_EXECUTABLE_OUTPUTS__QTD) "D:\work\CUG\V2\charger\Debug\charger.out"
	-$(RM) "D:\work\CUG\V2\charger\Debug\DSP2802x_GlobalVariableDefs.obj" "D:\work\CUG\V2\charger\Debug\SPKey.obj" "D:\work\CUG\V2\charger\Debug\adc.obj" "D:\work\CUG\V2\charger\Debug\main.obj" 
	-$(RM) ".\DSP2802x_GlobalVariableDefs.pp" ".\SPKey.pp" ".\adc.pp" ".\main.pp" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
