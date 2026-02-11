# Seeed Studio XIAO nRF Master Guide
*Covering XIAO nRF52840 (Sense) and XIAO nRF54L15 (Sense)*

> [!IMPORTANT]
> **Hardware Abstraction Rule**: Embedded code is NOT portable between chip generations! 
> - **nRF52** registers $\neq$ **nRF54** registers.
> - **Pin mappings** change physically between boards.
> - **ALWAYS** verify the DeviceTree (`.dts`) and Datasheet before running code. Do not blindly copy examples.

## 1. Quick Start: Which Board?

| Feature | XIAO nRF52840 (Sense) | XIAO nRF54L15 (Sense) |
| :--- | :--- | :--- |
| **Core** | Cortex-M4F @ 64MHz | **Cortex-M33 @ 128MHz** |
| **Wireless** | BLE 5.3, Mesh, Zigbee, Thread | **BLE 6.0**, Matter, Zigbee, Thread |
| **Low Power** | ~5$\mu$A Deep Sleep | **Ultra-Low Power** (System OFF) |
| **Battery** | BQ25101 + **P0.14** Divider Switch | Internal PMIC + **TPS22916** Load Switch |
| **Sensors** | LSM6DS3TR-C (IMU) + PDM Mic | LSM6DS3TR-C (IMU) + DMIC |

## 2. Development Frameworks

### A. nRF Connect SDK (Zephyr) - *Recommended*
The professional path for low-power and advanced networking (Matter/Zigbee).
-   **Board Names**:
    -   nRF52840: `xiao_ble` / `xiao_ble_sense`
    -   nRF54L15: `xiao_nrf54l15`
-   **Build Command**: `west build -b xiao_nrf54l15/nrf54l15/cpuapp`

### B. PlatformIO (Zephyr Wrapper)
Seeed provides a PlatformIO integration that wraps Zephyr.
-   **Use Case**: Rapid prototyping with VS Code but without the full NCS toolchain weight.
-   **Warning**: Might lag behind the latest NCS versions needed for Matter/BLE 6.0 features.

## 3. Hardware Quirks & "Gotchas"

### 🔋 Battery Voltage Monitoring

**nRF52840 (Sense)**:
-   **Mechanism**: A resistor divider connected to P0.31 (AIN7).
-   **Leakage Control**: Bottom of divider is controlled by **P0.14**.
-   **Code**:
    ```c
    // 1. Pull P0.14 LOW to enable divider
    gpio_pin_set(gpio0, 14, 0);
    // 2. Read ADC
    // 3. Set P0.14 High-Z to stop leakage
    ```

**nRF54L15 (Sense)**:
-   **Mechanism**: Uses a **TPS22916** load switch.
-   **Control**: Driver is integrated via Devicetree regulator.
-   **Code**:
    ```c
    static const struct device *const vbat_reg = DEVICE_DT_GET(DT_NODELABEL(vbat_pwr));
    regulator_enable(vbat_reg); // Enables the switch
    // Read ADC...
    regulator_disable(vbat_reg);
    ```

### 🧱 UART vs Battery (nRF54L15 Critical)
**The Trap**: If `CONFIG_SERIAL=y` is set (default in many samples), the nRF54L15 might **fail to boot** on battery power because it waits for the USB CDC connection.
-   **Fix**: Use overlays.
    -   `prj_uart.conf` (Debug): `CONFIG_SERIAL=y`, `CONFIG_UART_CONSOLE=y`
    -   `prj.conf` (Deploy): `CONFIG_SERIAL=n`

## 4. Peripherals & Storage

### QSPI Flash (nRF52840)
-   **Chip**: P25Q16H (2MB).
-   **Driver**: Use `nrfx_qspi` or Zephyr's flash API.
-   **Deep Sleep**: Requires sending standard QSPI commands (0xB9) to enter deep power-down if not using PM device handling.

### IMU (LSM6DS3TR-C)
-   **Driver**: Zephyr's `sensor` subsystem supports this OOB.
-   **Trigger**: Use `gpio_pin_interrupt_configure` on the INT1 pin for motion wakeup.

## 5. Advanced Protocols (nRF54L15)
The nRF54L15 is built for the **Matter** and **Zigbee** era.
-   **Matter**: Requires huge Flash/RAM. The nRF54L15's 1.5MB RRAM/Flash is ideal (vs nRF52840's 1MB).
-   **Zigbee**: NCS provides full ZBOSS stack support.
-   **Multi-Protocol**: Supports running BLE + Zigbee concurrently (Dynamic Multiprotocol).
