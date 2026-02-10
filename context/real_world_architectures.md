# Real-World Architectural Patterns

Analysis of professional open-source projects (OpenEarable v2, Seeed Xiao, MakerDiary) reveals superior patterns for organizing Zephyr/NCS projects.

## 1. The "Manager" Pattern (OpenEarable v2 Style)
Instead of a 2000-line `main.c`, professional firmware breaks functionality into "Managers" or "Services".

### Structure
```text
src/
├── main.c                  # Minimal init code (10-50 lines)
├── managers/
│   ├── battery_manager.c   # ADC, Fuel Gauge logic
│   ├── ble_manager.c       # Advertising, Connection callbacks
│   └── sensor_manager.c    # IMU polling, Data buffering
├── services/
│   └── led_service.c       # Indication patterns (Blink, Breathe)
└── include/
    └── managers/           # Public APIs
```

### Why use it?
-   **Isolation**: `ble_manager` doesn't need to know how `sensor_manager` works. They communicate via Zbus or Workqueues.
-   **Testability**: You can test the battery logic without bringing up the BLE stack.

## 2. Application-Specific Board Overlays (Seeed Xiao Style)
Do not edit the global `boards/` directory in the SDK. Instead, override hardware settings *per application*.

### Structure
```text
my_app/
├── src/
├── boards/
│   ├── nrf52840dk_nrf52840.overlay   # Overlay for DK
│   ├── xiao_ble.overlay              # Overlay for Xiao
│   └── nrf5340_audio_dk.conf         # Config for Audio DK
└── prj.conf
```

### Why use it?
-   **Portability**: The same source code runs on multiple boards just by adding a file in `boards/`.
-   **Cleanliness**: Your `app.overlay` stays empty or generic, while board-specific hacks (like "This board uses P0.05 for LED instead of P0.13") live in specific files.

## 3. The "Library" Pattern (Shared Drivers)
When you have a driver (like a complex sensor algorithm, e.g., Madgwick AHRS) used across multiple apps, do not copy-paste `src/`.

### Structure
```text
workspace/
├── apps/
│   └── my_app/
├── lib/
│   └── madgwick/
│       ├── CMakeLists.txt  # zephyr_library()
│       └── src/
└── west.yml                # Manifest
```

### Integration (CMakeLists.txt)
```cmake
# In my_app/CMakeLists.txt
add_subdirectory(../../lib/madgwick lib_madgwick)
target_link_libraries(app PRIVATE madgwick)
```

## 4. Multi-Core Configurations (nRF53/54)
For dual-core chips, configurations must be split.
-   `child_image/hci_rpmsg.conf`: Configures the **Network Core** (e.g., enable Coded PHY).
-   `prj.conf`: Configures the **Application Core** (e.g., Sensor logic).

**Rule**: Never try to set Controller-only configs (like `CONFIG_BT_CTLR_TX_PWR`) in `prj.conf` on an nRF53. It typically ignores them or fails. Put them in `child_image/hci_rpmsg.conf`.
