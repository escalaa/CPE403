################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/Coding/CCS8/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="D:/Oji/Documents/Coding/CCS8/hello_CC1350_LAUNCHXL_tirtos_ccs" --include_path="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/source/ti/posix/ccs" --include_path="D:/Coding/CCS8/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/include" --define=DeviceFamily_CC13X0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1397698895:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1397698895-inproc

build-1397698895-inproc: ../hello.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"D:/Coding/CCS8/xdctools_3_50_08_24_core/xs" --xdcpath="C:/ti/simplelink_cc13x0_sdk_2_30_00_20/source;C:/ti/simplelink_cc13x0_sdk_2_30_00_20/kernel/tirtos/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.simplelink:CC1350F128 -r release -c "D:/Coding/CCS8/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS" --compileOptions "-mv7M3 --code_state=16 --float_support=vfplib -me --include_path=\"D:/Oji/Documents/Coding/CCS8/hello_CC1350_LAUNCHXL_tirtos_ccs\" --include_path=\"C:/ti/simplelink_cc13x0_sdk_2_30_00_20/source/ti/posix/ccs\" --include_path=\"D:/Coding/CCS8/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/include\" --define=DeviceFamily_CC13X0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on  -std=c99 " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1397698895 ../hello.cfg
configPkg/compiler.opt: build-1397698895
configPkg/: build-1397698895


