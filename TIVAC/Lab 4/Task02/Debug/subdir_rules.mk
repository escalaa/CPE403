################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/Coding/CCS8/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/Oji/Documents/School/Fall2018/CPE 403/Labs/Lab 4/Task02" --include_path="D:/Coding/CCS8/TivaC123" --include_path="D:/Coding/CCS8/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=PART_TM4C123GH6PM --define=ccs="ccs" -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


