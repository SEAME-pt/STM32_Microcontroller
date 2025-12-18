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

# Viewing UART Debug Messages

Your firmware prints debug messages over UART (USART1, 115200 baud, 8N1).

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
 