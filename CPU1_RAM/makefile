################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./ADC_IO_Test_trigger_via_EPWM_GPIO.obj" \
"./f280013x_adc.obj" \
"./f280013x_codestartbranch.obj" \
"./f280013x_cputimers.obj" \
"./f280013x_defaultisr.obj" \
"./f280013x_epwm.obj" \
"./f280013x_globalvariabledefs.obj" \
"./f280013x_gpio.obj" \
"./f280013x_piectrl.obj" \
"./f280013x_pievect.obj" \
"./f280013x_sysctrl.obj" \
"./f280013x_tempsensorconv.obj" \
"./f280013x_usdelay.obj" \
"../280013x_generic_ram_lnk.cmd" \
"../f280013x_headers_nonBIOS.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Test_adc_Io_EPWM_GPIO.out \

EXE_OUTPUTS__QUOTED += \
"Test_adc_Io_EPWM_GPIO.out" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Test_adc_Io_EPWM_GPIO.out"

# Tool invocations
Test_adc_Io_EPWM_GPIO.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 -Ooff --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi -z -m"Test_adc_Io_EPWM_GPIO.map" --heap_size=0x100 --stack_size=0x200 --warn_sections -i"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/lib" -i"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --reread_libs --define=RAM --diag_wrap=off --display_error_number --xml_link_info="Test_adc_Io_EPWM_GPIO_linkInfo.xml" --entry_point=code_start --rom_model -o "Test_adc_Io_EPWM_GPIO.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "ADC_IO_Test_trigger_via_EPWM_GPIO.obj" "f280013x_adc.obj" "f280013x_codestartbranch.obj" "f280013x_cputimers.obj" "f280013x_defaultisr.obj" "f280013x_epwm.obj" "f280013x_globalvariabledefs.obj" "f280013x_gpio.obj" "f280013x_piectrl.obj" "f280013x_pievect.obj" "f280013x_sysctrl.obj" "f280013x_tempsensorconv.obj" "f280013x_usdelay.obj" 
	-$(RM) "ADC_IO_Test_trigger_via_EPWM_GPIO.d" "f280013x_adc.d" "f280013x_cputimers.d" "f280013x_defaultisr.d" "f280013x_epwm.d" "f280013x_globalvariabledefs.d" "f280013x_gpio.d" "f280013x_piectrl.d" "f280013x_pievect.d" "f280013x_sysctrl.d" "f280013x_tempsensorconv.d" 
	-$(RM) "f280013x_codestartbranch.d" "f280013x_usdelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

