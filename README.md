# STM32 ThreadX_Os

# Overview
This project implements a real-time speed sensor and CAN communication system on the STM32 B-U585I-IOT02A microcontroller using the ThreadX RTOS. The firmware acquires wheel speed pulses, computes RPM deterministically, and transmits telemetry data over the CAN bus. UART debug output is provided for monitoring and troubleshooting.

# Features

 - ThreadX RTOS: Manages concurrent threads for sensor reading and CAN transmission.

 - Wheel Speed Measurement: Uses a hardware timer to count pulses from a speed sensor and calculate RPM.

 - CAN Bus Communication: Transmits and receives messages using STM32 FDCAN peripheral.

 - UART Debug Output: Prints debug and status messages for development and troubleshooting.

# Directory Structure

### Core.
 - Main application source files
### Core/SRC.
 - Source files under ThreadX(threads, CAN, sensor logic, main.c, etc.)
### Core/INC.
 - Header files for application and hardware abstraction.
### Drivers.
 - STM32 HAL and CMSIS drivers.

# How It Works

### 1. Initialization:
 - ThreadX kernel and threads are initialized.
 - Two CAN queue are created for inter-thread communication.

### 2. Speed Measurement:
 - A dedicated sensor thread reads the timer counter, calculates wheel RPM, and enqueues a CAN message in a thread-safe manner.

### 3. CAN Transmission:
 - The CAN_TX thread dequeues messages and transmits them over the CAN bus, ensuring non-blocking and deterministic behavior, 
 - The CAN_RX thread enqueues received messages for further data processing.

### 4. Debugging:
 - UART output provides real-time feedback for development and troubleshooting.

# STM32 Pins Usage
 - FDCAN1_RX -> PB_8 (CAN_RX)
 - FDCAN1_TX -> PB_9 (CAN_TX)
 - TIM1_CH1  -> PA_8 (Sensor speed)

# How to Extend The Project
 - Implement a mechanism to process incoming data and perform the appropriate actions.
 - Integrate new thread to communicate via I2C to motors/servo.
 - Create a Heartbeat mechanism.
 - Integrate Watchdog timer that resets if the system "breaks".
 - Integration tests for CAN.
 - Latency tests

# Instructions to Build and Flash to STM32 Microcontroller

## Prerequisites
- ARM GCC toolchain (`arm-none-eabi-gcc`)
- CMake
- ST-Link tools (`stlink-tools`)

## Build Process

### 1. Create Build Directory
```shell
mkdir -p build
cd build
```

### 2. Configure CMake with ARM GCC Toolchain
```shell
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-arm-none-eabi.cmake ..
```

### 3. Compile the Project
```shell
make -j$(nproc)
```

## Flashing to STM32

### 4. Convert ELF to Binary Format
```shell
arm-none-eabi-objcopy -O binary ThreadX_Os.elf ThreadX_Os.bin
```

### 5. Verify ST-Link Connection
```shell
st-info --probe
```

Expected output should show your STM32 device information.

### 6. Erase Flash (Optional but Recommended)
```shell
st-flash erase
```

### 7. Write Binary to Flash Memory
```shell
st-flash --reset write ThreadX_Os.bin 0x08000000
```

**Parameters:**
- `0x08000000` - Flash memory start address for STM32
- `--reset` - Automatically reset the microcontroller after flashing

---

## Quick Rebuild and Flash
After initial setup, you only need:
```shell
cd build
make -j$(nproc)
arm-none-eabi-objcopy -O binary ThreadX_Os.elf ThreadX_Os.bin
st-flash erase
st-flash --reset write ThreadX_Os.bin 0x08000000
```
---

# Instructions to run the tests

Unit tests are implemented using the Unity framework and executed with Ceedling.
The tests focus exclusively on validating pure logic functions, isolated from hardware- and OS dependent code.

```shell
cd tests/unit

ceedling test:all

# Checking coverage
ceedling gcov:all
```

---

# UART Debug Messages

 - Debug messages are sent via USART1 at 115200 baud, 8N1.
 - Use a serial terminal (minicom, screen, etc.) to view output.

## 1. Connect the Board
- Use a USB cable if your board has a Virtual COM Port (e.g., ST-Link VCP)
- Or use a USB-to-Serial adapter connected to the board's UART TX/RX pins

## 2. Find the Serial Port
On Linux, run:
```sh
sudo dmesg | grep tty
```
Look for `/dev/ttyACM0`, `/dev/ttyUSB0`, or similar.

## 3. Open a Serial Terminal
You can use `minicom` or `screen`.

**minicom example:**
```sh
minicom -b 115200 -D /dev/ttyACM0
```

**screen example:**
```sh
screen /dev/ttyACM0 115200
```

## 4. UART Settings
- Baud rate: 115200
- Data bits: 8
- Parity: None
- Stop bits: 1

## 5. View Output
You should now see the debug messages printed by your firmware.

## 6. Kill UART Window
 - Press Ctrl+A then release both and press K
 - When prompted, press y to confirm exit.
