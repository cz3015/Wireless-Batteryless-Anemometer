################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Application/project_zero.obj: ../Application/project_zero.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" --cmd_file="c:/ti/simplelink/ble_sdk_2_02_01_18/src/config/build_components.opt" --cmd_file="C:/Users/ZhangChe/workspace_v8/project_zero_stack_cc2650/TOOLS/build_config.opt" --cmd_file="C:/Users/ZhangChe/workspace_v8/project_zero_stack_cc2650/TOOLS/ccs_compiler_defines.bcfg"  -mv7M3 --code_state=16 -me -O4 --opt_for_speed=0 --include_path="C:/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393/driverlib" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Profiles" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Application" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/ICallBLE" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Startup" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Board" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Scif" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Utils" --include_path="c:/ti/simplelink_academy_01_11_00_0000/modules/projects/support_files/Components/uart_log" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/examples/simple_peripheral/cc26xx/app" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/icall/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/roles/cc26xx" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/roles" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/dev_info" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/simple_profile/cc26xx" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/simple_profile" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/common/cc26xx" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/heapmgr" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/controller/cc26xx/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common/cc26xx" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/osal/src/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/sdata" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/saddr" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/icall/src/inc" --include_path="c:/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=USE_ICALL --define=POWER_SAVING --define=SBP_TASK_STACK_SIZE=700 --define=GAPROLE_TASK_STACK_SIZE=520 --define=HEAPMGR_SIZE=0 --define=Display_DISABLE_ALL --define=BOARD_DISPLAY_EXCLUDE_UART --define=xBOARD_DISPLAY_EXCLUDE_LCD --define=ICALL_MAX_NUM_TASKS=3 --define=ICALL_MAX_NUM_ENTITIES=6 --define=xdc_runtime_Assert_DISABLE_ALL --define=Xxdc_runtime_Log_DISABLE_ALL --define=MAX_NUM_BLE_CONNS=1 --define=CC26XX --define=xdc_FILE="\"project_zero.c\"" --define=UARTLOG_NUM_EVT_BUF=32 -g --c99 --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="Application/project_zero.d_raw" --obj_directory="Application" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Application/scTask.obj: ../Application/scTask.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" --cmd_file="c:/ti/simplelink/ble_sdk_2_02_01_18/src/config/build_components.opt" --cmd_file="C:/Users/ZhangChe/workspace_v8/project_zero_stack_cc2650/TOOLS/build_config.opt" --cmd_file="C:/Users/ZhangChe/workspace_v8/project_zero_stack_cc2650/TOOLS/ccs_compiler_defines.bcfg"  -mv7M3 --code_state=16 -me -O4 --opt_for_speed=0 --include_path="C:/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393/driverlib" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Profiles" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Application" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/ICallBLE" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Startup" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Board" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Scif" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Utils" --include_path="c:/ti/simplelink_academy_01_11_00_0000/modules/projects/support_files/Components/uart_log" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/examples/simple_peripheral/cc26xx/app" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/icall/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/roles/cc26xx" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/roles" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/dev_info" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/simple_profile/cc26xx" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/simple_profile" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/common/cc26xx" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/heapmgr" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/controller/cc26xx/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common/cc26xx" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/osal/src/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/sdata" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/saddr" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/icall/src/inc" --include_path="c:/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=USE_ICALL --define=POWER_SAVING --define=SBP_TASK_STACK_SIZE=700 --define=GAPROLE_TASK_STACK_SIZE=520 --define=HEAPMGR_SIZE=0 --define=Display_DISABLE_ALL --define=BOARD_DISPLAY_EXCLUDE_UART --define=xBOARD_DISPLAY_EXCLUDE_LCD --define=ICALL_MAX_NUM_TASKS=3 --define=ICALL_MAX_NUM_ENTITIES=6 --define=xdc_runtime_Assert_DISABLE_ALL --define=Xxdc_runtime_Log_DISABLE_ALL --define=MAX_NUM_BLE_CONNS=1 --define=CC26XX --define=xdc_FILE="\"scTask.c\"" --define=UARTLOG_NUM_EVT_BUF=32 -g --c99 --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="Application/scTask.d_raw" --obj_directory="Application" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Application/util.obj: C:/ti/simplelink/ble_sdk_2_02_01_18/src/common/cc26xx/util.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" --cmd_file="c:/ti/simplelink/ble_sdk_2_02_01_18/src/config/build_components.opt" --cmd_file="C:/Users/ZhangChe/workspace_v8/project_zero_stack_cc2650/TOOLS/build_config.opt" --cmd_file="C:/Users/ZhangChe/workspace_v8/project_zero_stack_cc2650/TOOLS/ccs_compiler_defines.bcfg"  -mv7M3 --code_state=16 -me -O4 --opt_for_speed=0 --include_path="C:/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393/driverlib" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Profiles" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Application" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/ICallBLE" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Startup" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Board" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Scif" --include_path="C:/Users/ZhangChe/workspace_v8/energy_harvester_ble_app_cc2650_5XD/Utils" --include_path="c:/ti/simplelink_academy_01_11_00_0000/modules/projects/support_files/Components/uart_log" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/examples/simple_peripheral/cc26xx/app" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/icall/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/roles/cc26xx" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/roles" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/dev_info" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/simple_profile/cc26xx" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/profiles/simple_profile" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/common/cc26xx" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/heapmgr" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/controller/cc26xx/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/target/_common/cc26xx" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/hal/src/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/osal/src/inc" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/sdata" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/services/src/saddr" --include_path="c:/ti/simplelink/ble_sdk_2_02_01_18/src/components/icall/src/inc" --include_path="c:/ti/tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=USE_ICALL --define=POWER_SAVING --define=SBP_TASK_STACK_SIZE=700 --define=GAPROLE_TASK_STACK_SIZE=520 --define=HEAPMGR_SIZE=0 --define=Display_DISABLE_ALL --define=BOARD_DISPLAY_EXCLUDE_UART --define=xBOARD_DISPLAY_EXCLUDE_LCD --define=ICALL_MAX_NUM_TASKS=3 --define=ICALL_MAX_NUM_ENTITIES=6 --define=xdc_runtime_Assert_DISABLE_ALL --define=Xxdc_runtime_Log_DISABLE_ALL --define=MAX_NUM_BLE_CONNS=1 --define=CC26XX --define=xdc_FILE="\"util.c\"" --define=UARTLOG_NUM_EVT_BUF=32 -g --c99 --gcc --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="Application/util.d_raw" --obj_directory="Application" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


