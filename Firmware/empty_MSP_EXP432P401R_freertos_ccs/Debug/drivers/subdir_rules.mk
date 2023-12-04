################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
drivers/%.obj: ../drivers/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1100/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/jorge/Desktop/GitHub/UTEP-Rover-Senior/firmware/empty_MSP_EXP432P401R_freertos_ccs" --include_path="C:/Users/jorge/Desktop/GitHub/UTEP-Rover-Senior/firmware/empty_MSP_EXP432P401R_freertos_ccs/Debug" --include_path="C:/ti/simplelink_msp432p4_sdk_3_40_01_02/source" --include_path="C:/ti/simplelink_msp432p4_sdk_3_40_01_02/source/third_party/CMSIS/Include" --include_path="C:/ti/simplelink_msp432p4_sdk_3_40_01_02/source/ti/posix/ccs" --include_path="C:/Users/jorge/Desktop/GitHub/UTEP-Rover-Senior/firmware/FreeRTOSv202212.00/FreeRTOSv202212.00/FreeRTOS/Source/include" --include_path="C:/Users/jorge/Desktop/GitHub/UTEP-Rover-Senior/firmware/FreeRTOSv202212.00/FreeRTOSv202212.00/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="C:/Users/jorge/Desktop/GitHub/UTEP-Rover-Senior/firmware/freertos_builds_MSP_EXP432P401R_release_ccs" --include_path="C:/ti/ccs1100/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="drivers/$(basename $(<F)).d_raw" --include_path="C:/Users/jorge/Desktop/GitHub/UTEP-Rover-Senior/firmware/empty_MSP_EXP432P401R_freertos_ccs/Debug/syscfg" --obj_directory="drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


