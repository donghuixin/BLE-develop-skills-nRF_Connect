# Knowledge Base Map

This project contains a comprehensive knowledge base for nRF Connect SDK development. Use this map to find the right context file.

## 1. The Constitution (Must Read)
-   **`ncs_engineering_standards.md`**: The Paradigm Shift, Golden Rules (Decoupling), and Philosophy. **Override all other files if in conflict.**
-   **`critical_safety_rules_antibricking.md`**: Mandatory safety checks for UICR, PMIC, and Multi-core to prevent hardware damage.

## 2. The Basics (Getting Started)
-   **`ble_core_guide.md`**: **BLE Bible**. Concepts, Stack, Roles, and Production Guide.
-   **`jlink_debug_guide.md`**: **Debugging**. RTT, Commander, and Recovering Bricked Boards.
-   **`zephyr_basics.md`**: Quick reference for DeviceTree syntax, Kconfig, and Driver Model concepts.
-   **`ncs_project_structure.md`**: Where files belong (`boards/`, `child_image/`, `src/`).

## 3. The Implementation (Writing Code)
-   **`zephyr_peripherals.md`**: "Gold Standard" copy-paste patterns for GPIO, I2C, SPI.
-   **`nordic_docs_index.md`**: Links to official Hardware Datasheets (Product Specs).

## 4. Real-World Architectures & Case Studies
-   **`xiao_ble_architecture.md`**: Analysis of Seeed Studio XIAO BLE. Dual-core strategies (Mbed vs FreeRTOS) and peripheral optimization.
-   **`meow_sense_tag_lessons.md`**: **HARD-WON LESSONS**. nRF5340 Dual-Core, BQ25120A PMIC, and rescue strategies.
-   **`real_world_architectures.md`**: Analysis of *OpenEarable*, *Seeed*, and *MakerDiary*. Comparison of module-based vs bare-chip designs.Source projects.

## 5. Examples
-   **`examples/i2c_sensor_read`**: Reference implementation of the "Hardware First" workflow.
