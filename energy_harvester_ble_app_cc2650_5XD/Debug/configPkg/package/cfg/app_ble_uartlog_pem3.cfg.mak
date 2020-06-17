# invoke SourceDir generated makefile for app_ble_uartlog.pem3
app_ble_uartlog.pem3: .libraries,app_ble_uartlog.pem3
.libraries,app_ble_uartlog.pem3: package/cfg/app_ble_uartlog_pem3.xdl
	$(MAKE) -f C:\Users\ZhangChe\workspace_v8\energy_harvester_ble_app_cc2650_5XD\TOOLS/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\ZhangChe\workspace_v8\energy_harvester_ble_app_cc2650_5XD\TOOLS/src/makefile.libs clean

