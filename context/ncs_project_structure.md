# nRF Connect SDK (NCS) Project Structure

Understanding where files go is critical for the build system (`west`/`cmake`) to find them.

## 📂 Standard Application Layout
```text
my_project/
├── CMakeLists.txt          # Main build script (project entry)
├── prj.conf                # Kconfig: Application-specific config
├── app.overlay             # DeviceTree: Hardware modifications
├── src/
│   └── main.c              # Source code
├── include/                # Public headers
│   └── my_lib.h
├── boards/                 # Board-specific overlays/configs
│   ├── nrf52840dk_nrf52840.overlay
│   └── nrf52840dk_nrf52840.conf
└── child_image/            # Multi-image configs (mcuboot, hci_rpmsg)
    └── mcuboot.conf
```

## 🧩 Key Files Explained

### `boards/*.overlay` vs `app.overlay`
-   **`app.overlay`**: Applied to *every* build, regardless of the board. Use for hardware logically attached to the app (e.g., a sensor on a custom shield).
-   **`boards/<BOARD>.overlay`**: Automatically applied *only* when building for that specific board. Use for mapping pins to board-specific routing.

### `child_image/`
-   **What**: Configuration for "Child Images" like MCUBoot or the Network Core (hci_rpmsg).
-   **Why**: `prj.conf` ONLY configures the Application Core. It does NOT affect the Network Core or Bootloader.
-   **Example**: To enable logging in the Network Core, create `child_image/hci_rpmsg.conf` and add `CONFIG_LOG=y`.

### `west.yml` (Advanced)
-   Defines the project manifest (repositories and versions). Only present in the root of the workspace or valid Zephyr module.

## 🚀 Advanced Patterns (See `real_world_architectures.md`)

### 1. The "Manager" Structure
For complex apps, split `main.c` into Modules/Managers:
```text
src/
  ├── main.c           # Orchestrator only
  ├── modules/         # Independent logic units
  │   ├── ble.c
  │   └── sensors.c
```

### 2. Custom Board Support
If you have a custom PCB, create a board definition in `boards/arm/my_custom_board` and reference it with `west build -b my_custom_board`. Don't monkey-patch standard boards.

