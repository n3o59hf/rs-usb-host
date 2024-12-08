# rs-usb-host

Fork of [ps2x2pico](https://github.com/No0ne/ps2x2pico) by [No0ne](https://github.com/No0ne).

This fork sends input data over UART, removing all PS2 capabilities and focusing on a streamlined protocol for transmitting keyboard and mouse.

## Features

- **UART-Based Data Transmission:**
  - Sends data in a structured format over UART for further processing.
  - Includes checksum validation to ensure data integrity.
- **Queue-Based Packet Handling:**
  - Utilizes a queue to manage and process outgoing packets efficiently.
- **Keyboard and Mouse Support:**
  - Sends key presses, mouse movements, and button states.

## UART Data Format

```
[Start Byte] [Length] [Payload]      [Checksum]
0x7E         0x03     0x01 0x02 0x03 0x06
```

### Explanation:
- **Start Byte (0x7E):** Indicates the beginning of a packet.
- **Length:** Number of bytes in the payload.
- **Payload:** The actual data being sent (e.g., key presses, mouse movements).
- **Checksum:** XOR of all payload bytes to validate the data.

## Packet Types

- **Keyboard Packet (ID: 0x01):**
  - Payload Format: `[0x01, KeyCode, KeyState, Modifiers]`
    - `KeyCode`: The key identifier.
    - `KeyState`: `1` for pressed, `0` for released.
    - `Modifiers`: Bitfield for Shift, Ctrl, Alt, etc.

- **Mouse Packet (ID: 0x02):**
  - Payload Format: `[0x02, Buttons, X, Y, Z]`
    - `Buttons`: Mouse button states.
    - `X, Y, Z`: Relative mouse movement in the X, Y, and Z axes.

- **Log Packet (ID: 0x03):**
  - Payload Format: `[0x03, LogString...]`
    - `LogString`: String containing the log message.


## Building

See general steps from prepare.bat and build.bat.

1. **Build the Project:**
   Ensure you have the Pico SDK **1.5.1** set up, then build the project using CMake:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
2. **Flash to RP2040:**
   Copy the `.uf2` file to the RP2040 when it is in UF2 mode.
3. **Connect UART:**
   Connect your UART interface to the appropriate TX and RX pins on the RP2040 (by default pins 4 and 5 are used).

## License

This project inherits the license from the original [ps2x2pico](https://github.com/No0ne/ps2x2pico).
