# rs-usb-host

Fork of [ps2x2pico](https://github.com/No0ne/ps2x2pico) by [No0ne](https://github.com/No0ne).

This fork sends most of the data about inputs via UART, with all of ps2 capabilities removed.

## Features

- **UART-Based Data Transmission:**
  - Sends data in a structured format over UART for further processing.
  - Includes checksum validation to ensure data integrity.
- **Queue-Based Packet Handling:**
  - Utilizes a queue to manage and process outgoing packets efficiently.
- **Keyboard and Mouse Support:**
  - Sends key presses, mouse movements, and button states.
- **Logging Capability:**
  - Supports sending logs over UART for debugging or monitoring purposes.