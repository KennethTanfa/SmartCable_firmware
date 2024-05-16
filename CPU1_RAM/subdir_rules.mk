################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 -Ooff --include_path="D:/WORK/PROJECTS/04_SmartCable_ROSSINI_ENERGY_RTE/Workspace/CCS_smSmartcable_testing_phase/gpio_test_toggle/Test_adc_Io_EPWM_GPIO" --include_path="C:/ti/c2000/C2000Ware_5_01_00_00/device_support/f280013x/common/include" --include_path="C:/ti/c2000/C2000Ware_5_01_00_00/device_support/f280013x/headers/include" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 -Ooff --include_path="D:/WORK/PROJECTS/04_SmartCable_ROSSINI_ENERGY_RTE/Workspace/CCS_smSmartcable_testing_phase/gpio_test_toggle/Test_adc_Io_EPWM_GPIO" --include_path="C:/ti/c2000/C2000Ware_5_01_00_00/device_support/f280013x/common/include" --include_path="C:/ti/c2000/C2000Ware_5_01_00_00/device_support/f280013x/headers/include" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


