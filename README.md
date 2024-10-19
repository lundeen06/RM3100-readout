# RM3100 Magnetometer Readout

This project utilizes an Arduino Uno to interface with an RM3100 Magnetometer sensor via I2C configuration. It continuously reads and outputs magnetometer data when the sensor is connected, and provides a visual indicator using the onboard LED when the sensor is not detected.

## Features

- Reads magnetometer data every ~10ms when the RM3100 sensor is connected
- Outputs readings in a structured format via serial communication
- Visual indicator (LED blinks) when the sensor is not detected
- Automatic reconnection attempts when the sensor is disconnected

## Hardware Requirements

1. Arduino Uno board
2. RM3100 Magnetometer sensor
3. Two 10K ohm resistors (for I2C pull-ups)

## Wiring

Connect the RM3100 Magnetometer to the Arduino Uno as follows:

- VCC to 3.3V
- GND to GND
- SDA to A4 (or SDA pin if your Arduino has dedicated SDA/SCL pins)
- SCL to A5 (or SCL pin if your Arduino has dedicated SDA/SCL pins)

Additionally, you need to add pull-up resistors:
- Connect a 10K ohm resistor from SDA to 3.3V
- Connect a 10K ohm resistor from SCL to 3.3V

These pull-up resistors are crucial for proper I2C communication. They ensure the bus lines are pulled high when idle, which is necessary for the I2C protocol to function correctly.

## Software Dependencies

- Arduino IDE
- Wire library (included with Arduino IDE)
- Custom Rm3100 library (included in this project)

## Setup

1. Clone this repository or download the project files.
2. Open the `RM3100-readout.ino` file in the Arduino IDE.
3. Ensure that the Rm3100.h and Rm3100.cpp files are in the same directory as the .ino file.
4. Connect your Arduino Uno to your computer.
5. Select the correct board and port in the Arduino IDE.
6. Upload the sketch to your Arduino Uno.

## Usage

1. After uploading the sketch, open the Serial Monitor in the Arduino IDE.
2. Set the baud rate to 115200.
3. If the sensor is correctly connected, you should see magnetometer readings in the format:
   ```
   MAG [timestamp] [x] [y] [z]
   ```
   Where [timestamp] is the time since the Arduino started in milliseconds, and [x], [y], [z] are the magnetic field strengths in µT (micro-Tesla) for each axis.

4. If the sensor is not detected, you will see the message "Magnetometer not connected!" in the Serial Monitor, and the onboard LED will blink twice quickly, pause, and repeat.

## Troubleshooting

- If you're not seeing any output in the Serial Monitor, ensure that the baud rate is set to 115200.
- If you're seeing "Magnetometer not connected!" messages:
  - Check your wiring connections
  - Ensure the sensor is powered correctly
  - Verify that you're using a compatible RM3100 Magnetometer sensor
  - Double-check that the pull-up resistors are correctly installed
- If the communication is unstable:
  - Ensure that the pull-up resistors are properly connected
  - Try shortening the wires between the Arduino and the sensor
  - Check for any nearby sources of electromagnetic interference

## Notes on I2C Pull-up Resistors

The 10K ohm pull-up resistors on the SDA and SCL lines are essential for reliable I2C communication. They serve several purposes:

1. They ensure the I2C bus lines are in a known (high) state when idle.
2. They allow for multi-device communication on the same bus.
3. They help maintain signal integrity and reduce noise.
4. They comply with the I2C specification for proper bus operation.

While some setups might work without external pull-ups (relying on internal MCU pull-ups), adding these resistors is a best practice that ensures more reliable and robust I2C communication, especially in noisy environments or with longer cable runs.