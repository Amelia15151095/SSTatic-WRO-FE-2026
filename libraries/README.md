# Software Environment and Libraries

The autonomous vehicle software was developed using Arduino-style C++ for the ESP32-based EVOX1 controller.

## Development Environment

- Arduino IDE 2.x
- ESP32 board support package by Espressif Systems
- EVO Arduino library
- Libraries required by the final ToF and IMU implementation

## EVO Library

The EVO Arduino library was installed manually from the EVO Arduino GitHub releases.

During development, compatibility issues occurred between some ESP32 board-package versions and the EVO library. These had to be resolved before the code could compile and upload correctly.

## Upload Procedure

1. Connect the EVOX1 controller to the computer using a USB data cable.
2. Open the Open Challenge `.ino` file in Arduino IDE.
3. Ensure the required libraries are installed.
4. Select the correct ESP32/EVO board configuration.
5. Select the correct USB serial port.
6. Compile the program.
7. Upload the compiled program to the controller.
8. Reset the controller if required.
9. Use Serial Monitor at 115200 baud to inspect debugging and sensor output.
10. Test the robot and adjust calibration values where necessary.

## Upload Problems Encountered

During development we encountered several upload-related problems, including:

- charge-only or unreliable USB cables;
- incorrect serial-port selection;
- the controller requiring a reset before uploading;
- BOOT-button reliability problems;
- ESP32 board-package compatibility;
- EVO library compatibility;
- slow or failed uploads.

These issues were separated from navigation bugs because successful compilation does not necessarily mean that the program has successfully reached the controller.
