# Magnetometer Readout

This project utilizes an Arduino Uno to interface with an RM3100 Magnetometer sensor via I2C configuration. It continuously reads and outputs magnetometer data when the sensor is connected, and provides a visual indicator using the onboard LED when the sensor is not detected.

## Features

- Reads magnetometer data every ~10ms when the RM3100 sensor is connected
- Outputs readings in a structured format via serial communication
- Visual indicator (LED blinks) when the sensor is not detected
- Automatic reconnection attempts when the sensor is disconnected

## Hardware Requirements

1. Arduino Uno board
2. RM3100 Magnetometer sensor
3. Connecting wires

## Wiring

Connect the RM3100 Magnetometer to the Arduino Uno as follows:

- VCC to 3.3V
- GND to GND
- SDA to COPI
- SCL to CLK

## Software Dependencies

- Arduino IDE
- Wire library (included with Arduino IDE)
- Custom Rm3100 library (included in this project)

## Setup

1. Clone this repository or download the project files.
2. Open the `magnetometer-readout.ino` file in the Arduino IDE.
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

## Customization

You can adjust the following parameters in the code:

- `BLINK_COUNT`: Number of LED blinks when sensor is not connected (default: 2)
- `BLINK_DURATION`: Duration of each blink in milliseconds (default: 100)
- `WAIT_DURATION`: Wait time between blink sequences in milliseconds (default: 2000)

## Contributing

Feel free to fork this project and submit pull requests with any improvements or bug fixes.

## License

This project is open source and available under the [MIT License](LICENSE).