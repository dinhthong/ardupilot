# References
- Ardupilot RP: RossosHEX/ardupilot. 
https://github.com/RossosHEX/ardupilot/commit/857a43ce5e396c895b0cda86ca3a9ada895c805d
- Hardware: Pixhawk 2.4.8 (FMUv3)
# Build Ardupilot bootloader for STM32Discovery

# Build example sketches.
- Step 1: setup build environment: https://ardupilot.org/dev/docs/building-setup-linux.html#building-setup-linux
- Step 2: https://github.com/ArduPilot/ardupilot/blob/master/BUILD.md
- Step 3: Detail tutorial on how to build example sketch: https://ardupilot.org/dev/docs/learning-ardupilot-the-example-sketches.html
- Video build and upload example sketch (by me): https://www.youtube.com/watch?v=Spb75Pd3z_c&ab_channel=Edinhthong

# Several Ardupilot learning and research articles, links
- https://ardupilot.org/dev/docs/learning-the-ardupilot-codebase.html
- https://desosa.nl/projects/ardupilot/2020/03/19/what-makes-ardupilot-soar-an-architectural-overview

# Todos
- Buy Omnibus F4 -> build and load Ardupilot firmware
- Debugging system with ST-Link...
- Add a new mode to system https://ardupilot.org/dev/docs/apmcopter-adding-a-new-flight-mode.html
- Port to my STM32F4 Discovery or DIY_Quadcopter project. Base on this work: https://discuss.ardupilot.org/t/porting-to-stm32f407ve-512kb-flash-192-4kb-sram/36903/7

# Build new bootloader for your board
https://ardupilot.org/dev/docs/porting.html#step-4-upload-an-ardupilot-compatible-bootloader-to-the-board
<pre><code>
./waf configure --board stm32f407vg --bootloader
./waf clean
./waf bootloader
</code></pre>
After building the bootloader, load it though STM32 ST-Link Utility
