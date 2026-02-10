# BLE Core Guide: From Concept to Product

*Source: Internal Knowledge Base*

## Part 1: BLE Cognitive Foundation

### 1.1 Definition & Differences
BLE (Bluetooth Low Energy) focuses on "Ultra Low Power", distinct from Classic Bluetooth (BR/EDR).

| Dimension | BLE | Classic Bluetooth |
| :--- | :--- | :--- |
| **Power** | $\mu$A level, coin cell lasts months/years | mA level, hours/days |
| **Rate** | 1-2 Mbps (Small packets) | ~3 Mbps (Audio/Video stream) |
| **Usage** | Sensors, Wearables, IoT | Headsets, Speakers, File Transfer |
| **Connect** | Fast connect/disconnect | Long-term connection |

### 1.2 Protocol Stack Architecture
-   **Controller**: RF, Baseband, Link Layer (States: Standby, Adv, Scan, Connected).
-   **Host**: L2CAP, SM (Security), ATT/GATT (Data), GAP (Roles).
-   **Application**: Business logic calling Host APIs.

### 1.3 Roles & Interaction
-   **Advertiser (Slave)**: Broadcasts presence (e.g., Smart Band).
-   **Scanner (Master)**: Listens for devices (e.g., Phone).
-   **Interaction Flow**:
    1.  **Broadcast**: Slave sends ADV packets.
    2.  **Connect**: Master sends REQ, negotiates parameters (Interval, Latency).
    3.  **Data**: GATT Read/Write/Notify.
    4.  **Disconnect**: Release resources, return to Sleep/Adv.

## Part 2: Development Environment

### 2.1 Hardware Selection
-   **Nordic**: nRF52832 (Cost/Perf balanced), nRF52840 (High-end, Multi-protocol).
-   **TI**: CC2640R2F (Medical/Wearable).
-   **Dialog**: DA14531 (Ultra-low cost/size).
-   **Selection**: Pure Chip (High vol, RF design needed) vs Module (Fast time-to-market).

### 2.2 Software Setup (Nordic NCS)
-   **IDE**: VS Code + nRF Connect Extension.
-   **SDK**: nRF Connect SDK (Zephyr + SoftDevice Controller).
-   **Tools**:
    -   **West**: Project management.
    -   **J-Link**: Debug/Flash.
    -   **nRF Connect Desktop/Mobile**: Testing counterpart.
    -   **nRF Sniffer**: Air packet capture.

## Part 3: Core Development Practice

### 3.1 Basic Functionality (Sensor Example)
-   **Advertising**: Set `Connectable`. Payload: Name ("Temp_Sensor"), UUID.
-   **Connection**: Handle `connected` and `disconnected` callbacks (restart Adv on disconnect).
-   **GATT Services**:
    -   **Service**: Temp/Humidity Service (UUID: `0xFFF0`).
    -   **Char 1**: Temp (UUID `0xFFF1`, Read + Notify).
    -   **Char 2**: Humi (UUID `0xFFF2`, Read + Notify).
    -   **Char 3**: Control (UUID `0xFFF3`, Write).
-   **Data Transfer**:
    -   **Slave**: `bt_gatt_notify()` to push data.
    -   **Master**: Write to Control Char to trigger actions.

### 3.2 Advanced Features
-   **Security**: Pairing (Exchange Keys) + Bonding (Store Keys). Levels: Open, Encrypted, Authenticated.
-   **Power Optimization**:
    -   **Hardware**: DCDC regulator, matching network.
    -   **Software**: Deep Sleep (System ON/OFF), close unused peripherals (UART/ADC).
    -   **Params**: Increase Connection Interval (e.g., 100ms -> 1s), use Slave Latency.
-   **Multi-Connection**: Master connects to multiple Slaves. Enable `CONFIG_BT_MAX_CONN`.

### 3.3 Case Study: Smart Band
-   **Arch**: nRF52832 + MAX30102 (HR) + ADXL345 (Accel) + OLED.
-   **Flow**:
    -   Sensors wake via Timer/GPIO -> Read Data -> Buffer.
    -   BLE Notification -> Phone application.
    -   Phone Notification (Call/WeChat) -> Write to Band -> OLED Display.

## Part 4: Debugging & Production

### 4.1 Debugging
-   **Connectivity**: Check Adv Data, RF power.
-   **Data**: Check Notification Enable (CCCD), MTU size (default 23 bytes).
-   **Sniffer**: Mandatory for protocol errors (ACK missing, Sequence wrong).
-   **Power**: Power Analyzer to profile current.

### 4.2 Common Issues
-   **Can't Scan**: Check Antenna, Power, "Connectable" flag.
-   **Frequent Disconnect**: RF interference, Supervision Timeout too short.
-   **High Power**: Unused UART/Logging enabled, Floating pins.

### 4.3 Production Testing
-   **RF**: Tx Power, Rx Sensitivity.
-   **Functional**: Auto-test scripts for Adv/Connect/Data.
-   **Reliability**: Aging, Reboot loops.
-   **Certification**: BQB (Bluetooth SIG), FCC/CE/SRRC.

## Part 5: Trends
-   **LE Audio**: Low latency, broadcast audio (Auracast).
-   **Long Range**: Coded PHY (1km+).
-   **AoA/AoD**: Direction Finding (Indoor positioning).
