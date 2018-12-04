# invoke SourceDir generated makefile for rom.pem3
rom.pem3: .libraries,rom.pem3
.libraries,rom.pem3: package/cfg/rom_pem3.xdl
	$(MAKE) -f D:\Oji\Documents\Coding\CCS8\adc_window_monitor_launchpad_tirtos/src/makefile.libs

clean::
	$(MAKE) -f D:\Oji\Documents\Coding\CCS8\adc_window_monitor_launchpad_tirtos/src/makefile.libs clean

