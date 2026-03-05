# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

STM32F103xB (Cortex-M3) embedded firmware project using STM32 HAL library and CMake build system. Generated and managed with STM32CubeMX.

## Build Commands

### Configure and Build
```bash
# Configure with Debug preset
cmake --preset Debug

# Build Debug configuration
cmake --build build/Debug

# Configure and build Release
cmake --preset Release
cmake --build build/Release
```

### Build Artifacts
- ELF file: `build/Debug/STM32Demo.elf` or `build/Release/STM32Demo.elf`
- Map file: `build/Debug/STM32Demo.map` or `build/Release/STM32Demo.map`

### Clean Build
```bash
rm -rf build/Debug
# or
rm -rf build/Release
```

## Project Architecture

### Hardware Configuration
- **MCU**: STM32F103xB (Cortex-M3, 128KB Flash, 20KB RAM)
- **Clock**: HSI (8MHz internal oscillator, no PLL)
- **GPIO**: PA6 configured as LED output (LED_Pin)

### Directory Structure
- `Core/Src/main.c`: Application entry point and main loop
- `Core/Inc/main.h`: Application header with pin definitions
- `Core/Src/stm32f1xx_hal_msp.c`: HAL MSP initialization callbacks
- `Core/Src/stm32f1xx_it.c`: Interrupt handlers
- `Drivers/STM32F1xx_HAL_Driver/`: STM32 HAL library sources
- `Drivers/CMSIS/`: ARM CMSIS headers and STM32 device files
- `cmake/`: Build system configuration
  - `gcc-arm-none-eabi.cmake`: ARM GCC toolchain file
  - `stm32cubemx/CMakeLists.txt`: Auto-generated HAL driver build config
- `startup_stm32f103xb.s`: Startup assembly code
- `STM32F103XX_FLASH.ld`: Linker script defining memory layout
- `STM32Demo.ioc`: STM32CubeMX project file

### Code Organization Pattern
STM32CubeMX generates code with `USER CODE BEGIN/END` comment blocks. Always place custom code within these blocks to preserve changes when regenerating from .ioc file:
```c
/* USER CODE BEGIN Includes */
// Your includes here
/* USER CODE END Includes */
```

### Build System
- CMake 3.22+ with Ninja generator
- Toolchain: `arm-none-eabi-gcc` (must be in PATH)
- Two-level CMake structure:
  1. Root `CMakeLists.txt`: User application code
  2. `cmake/stm32cubemx/CMakeLists.txt`: Auto-generated HAL/CMSIS sources (do not edit manually)

### Compiler Flags
- Debug: `-O0 -g3`
- Release: `-Os -g0`
- Common: `-mcpu=cortex-m3 -Wall -fdata-sections -ffunction-sections`
- Linker: `--specs=nano.specs` (newlib-nano), `--gc-sections` (dead code elimination)

## STM32CubeMX Integration

When modifying hardware configuration:
1. Open `STM32Demo.ioc` in STM32CubeMX
2. Make changes to peripherals, clocks, or pins
3. Generate code (preserves USER CODE sections)
4. Rebuild project

The `cmake/stm32cubemx/CMakeLists.txt` file is auto-generated - do not edit manually.

## Adding New Source Files

Add user source files to root `CMakeLists.txt`:
```cmake
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    Core/Src/my_module.c
)
```

Add user include directories:
```cmake
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    Core/Inc/my_module
)
```

## Debugging

The project uses STM32Cube IDE tooling:
- `cube-cmake`: CMake wrapper for STM32Cube
- `starm-clangd`: clangd language server for ARM embedded development
- Compile commands exported to `build/Debug/compile_commands.json` for IDE integration

## Memory Layout

Defined in `STM32F103XX_FLASH.ld`:
- Flash: 128KB starting at 0x08000000
- RAM: 20KB starting at 0x20000000
