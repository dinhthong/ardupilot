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

# Understand how flight mode changes
- Mode change is commanded by many sources: from user RC input, GCS, or maybe system failsafe, condition to change flight mode in code...
- The RC_Chanel for each vehicle is implemeted differently, that's why we have RC_Channel_Copter inherits from RC_channel
## Explanation for RC input:
- The Copter::rc_loop() reads the current RC input (at 100Hz)
- We have many rc channels, RC_Channel::read_mode_switch() reads the mode switch , and calls 'void RC_Channel::read_mode_switch()' -> void RC_Channel_Copter::mode_switch_changed() -> copter.set_mode(). 
+ set_mode(new_mode, reason) checks if the switch conditions are meet and switch to the new flight mode.
