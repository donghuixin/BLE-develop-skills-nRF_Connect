# XIAO BLE (nRF52840) Architecture & System Design

*Source: Research Report on Seeed Studio XIAO BLE Series*

## 1. Hardware Variants & Evolution
The XIAO BLE platform (21 x 17.5 mm) leverages the nRF52840 SoC (64MHz Cortex-M4F, 1MB Flash, 256KB RAM) with an additional **2MB QSPI Flash**.

### Variant Comparison
| Feature | XIAO BLE (Standard) | XIAO BLE Sense | XIAO BLE Plus |
| :--- | :--- | :--- | :--- |
| **Sensors** | None | 6-axis IMU + PDM Mic | None |
| **GPIOs** | 11 (PWM) | 11 (PWM) | 20 (extra pins) |
| **Special** | Basic BLE | Edge AI / TinyML | I2S / Industrial I/O |
| **Charger** | BQ25101 | BQ25101 | BQ25101 |

## 2. Software Ecosystems: The Dual-Path Strategy
XIAO BLE supports two distinct Arduino cores, each optimized for different needs:

### A. Non-Mbed Core (Adafruit Architecture)
-   **Underlying OS**: FreeRTOS.
-   **Philosophy**: Efficiency & Determinism.
-   **Low Power**: 2.4 $\mu$A - 5 $\mu$A deep sleep current.
-   **Hardware Control**: Supports up to 16 PWM channels (direct `nrfx` access).
-   **Stack**: TinyUSB + LittleFS.

### B. Mbed-Enabled Core
-   **Underlying OS**: Mbed OS (Real-time OS abstraction).
-   **Philosophy**: Robustness & Standardization.
-   **TinyML**: Better synchronization for high-speed sensor/mic data streams (e.g., TensorFlow Lite Micro).
-   **Trade-off**: Higher power consumption and potential latency due to abstraction layers.

## 3. High-Performance Feature Implementation

### Low Power Battery Monitoring
To prevent leakage current through the resistor divider, XIAO BLE uses a **Logic Switch**:
-   **Hardware**: Divider bottom connected to **P0.14** (Logic Switch).
-   **Software Logic**:
    1.  Pull **P0.14 LOW** to enable the circuit.
    2.  Read ADC value.
    3.  Set **P0.14 to High-Z/High** to disable and stop leakage.

### Audio & IMU Data (Sense Model)
-   **PDM Mic**: Uses Hardware PDM module + **Double Buffering**.
-   **IMU**: LSM6DS3TR-C.
-   **Driver Pattern**: Encapsulated C++ classes with `begin()` and thread-safe data access.

## 4. Professional Evolution: Zephyr RTOS
Zephyr is the "Industrial Path" for XIAO BLE:
-   **Configuration-Driven**: Hardware features described in **Devicetree (.dts / .overlay)**.
-   **Separation**: Decouples Application logic from Pin Mappings via **Aliases**.
-   **Precision**: `prj.conf` allows fine-grained RAM/Stack optimization, crucial for nRF52840's 256KB RAM.

## 5. Engineering Standards & Best Practices
-   **Naming**: `SCREAMING_SNAKE` for constants (e.g., `PIN_VBAT`), `lowerCamelCase` for APIs.
-   **HAL Usage**: Use `nrfx_qspi.h` or `nrfx_pdm.h` for performance-critical paths (e.g., zero-CPU load audio sampling via DMA).
-   **UF2 Bootloader**: Files are automatically converted to `.uf2` via Python scripts after compilation for easy "Drag & Drop" flashing.

## 6. Maintenance & Knowledge Management
-   **Version Traceability**: Associate docs with specific GitHub commits.
-   **Scenario-Driven**: Choose core based on "Low Power" (Non-Mbed) vs "Complex ML" (Mbed).
-   **Community-Powered**: Extract FAQs from forums (e.g., I2C pin conflicts) into standard KB entries.
