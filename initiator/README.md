# Light Pattern Control -- Initiator

This is an example ESP-NOW initiator. When you press the `BOOT` button on the board, it will send a message to a detected responder to toggle the requested LED segment.

## Setup

Run `idf.py set-target esp32c6` to set the ESP32-C6 as the target device for the initiator.

Run `idf.py menuconfig` and select `Example Configuration` to configure the initiator.

By default the initator will listen for `$RESPONDER/GROUP_0` from the responder, and send `$INITIATOR/GROUP_0/0` messages to the responder. These are the default values.

You should modify the GPIO pin value in Menuconfig to control the required LED segment using the initiator.

**Important:** make sure the same values for initiator and responder messages are used. Otherwise, the initiator and responder will not be able to communicate with each other.

## Building

After you have finished using menuconfig, run `idf.py build` to build the program.

If the program builds successfully, run `idf.py -p COMx flash monitor` to flash the code to the ESP32-C6 and monitor the output from the debug serial port. Substitute `COMx` with the actual COM port the ESP32-C6 is using. If you have only one ESP32 device connected to your computer, you can omit `-p COMx`. `idf.py` will automatically detect the correct COM port to use.

## Running the program

On start-up, the ESP32-C6 initiator listen for a broadcast message from the responder to discover its unique MAC address. This is used to ID the responder to send messages to. When the `BOOT` button is pressed, the initiator will send a command to the responder to toggle the requested LED segment.

## Testing Options

### Setting the Wi-Fi data rate

The Wi-Fi data rate used by the initiator can be configured. By default the initiator sends data at the Wi-Fi 1 Mbits/s data rate.

To change this, run `idf.py menuconfig`, select `Example Configuration` and select the `Wi-Fi Data Rate used to send packets` option to show the possible data rates that can be used. In this example the rates can be set from 1 Mbits/s (the default) to 54 Mbits/s.
