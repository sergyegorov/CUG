################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
DSP2802x_headers/cmd/DSP2802x_Headers_nonBIOS.out: D:/work/CUG/V2/charger/DSP2802x_headers/cmd/DSP2802x_Headers_nonBIOS.cmd $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: Linker'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g --define="_DEBUG" --define="LARGE_MODEL" --diag_warning=225 --large_memory_model --obj_directory="D:/work/CUG/V2/charger/Debug" -z -m"D:/work/CUG/V2/charger/DSP2802x_headers/cmd/Debug/charger.map" --stack_size=300 --warn_sections -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/lib" -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/include" -i"D:/work/CUG/V2/charger/charger" -i"D:/work/CUG/V2/charger" -i"C:/CCStudio_v3.1/C2000/cgtools/lib" --reread_libs --rom_model -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


