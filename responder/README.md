# Light Pattern Control -- Responder

This is an example ESP-NOW responder. It will periodically broadcast a message that initiators will listen to find the responder.

When the responder receives a message from an initiator, it will check whether it is a valid message, and then toggle the LED segment requested by the initiator.

## Setup

Run `idf.py set-target esp32c6` to set the ESP32-C6 as the target device for the responder.

Run `idf.py menuconfig` and select `Example Configuration` to configure the responder.

By default the responser will broadcast `$RESPONDER/GROUP_0` and listen for `$INITIATOR/GROUP_0/0` from initiators. These are the default values. You can modify the values using Menuconfig as required.

**Important:** make sure the same values for initiator and responder messages are used. Otherwise, the responder and initiators will not be able to communicate with each other.

## Building

After you have finished using menuconfig, run `idf.py build` to build the program.

If the program builds successfully, run `idf.py -p COMx flash monitor` to flash the code to the ESP32-C6 and monitor the output from the debug serial port. Substitue `COMx` with the actual COM port the ESP32-C6 is using. If you have only one ESP32 device connected to your computer, you can omit `-p COMx`. `idf.py` will automatically detect the correct COM port to use.

## Running the program

On start-up, the ESP32-C6 responder will gradually light up all the LED segments. This will let you verify that the LEDs are working as expect. At the end, all the LEDs are turned off and the responder waits for initiators to send commands to toggle the requested LED segment.

### Wi-Fi data rate used by the initiator

The Wi-Fi data rate used by the initiator to send data is now shown when a request from the initiator is received.

## Testing options

### Testing RSSI Filtering

Each packet accepted by the responder includes extra information on the packet. One piece of information provided is the RSSI (Received signal strength indicator) which is a value in dB. With this information, the responder can choose to act based on the value of the RSSI. For example, it can ignore packets from initiators whose RSSI value is too low.

To try this, run `idf.py menuconfig`, select `Example Configuration` and toggle the `Enable filtering of initiator messages based on signal strength (RSSI)` option. When enabled, an extra option, `RSSI filter value to use` is displayed. The default value is -30 dB. A value from -50 db to -20 dB can be used.

Once enabled, rebuild and flash the new code to the responder. When the initiators received RSSI value falls below the configure filter value, the initiators message will be ignored, and the LED segment will not toggle.
