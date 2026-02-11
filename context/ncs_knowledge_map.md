# Knowledge Base Map

This project contains a comprehensive knowledge base for nRF Connect SDK development. Use this map to find the right context file.

## 1. The Constitution (Must Read)
-   **`ncs_engineering_standards.md`**: The Paradigm Shift, Golden Rules (Decoupling), and Philosophy. **Override all other files if in conflict.**
-   **`critical_safety_rules_antibricking.md`**: Mandatory safety checks for UICR, PMIC, and Multi-core to prevent hardware damage.

## 2. The Basics (Getting Started)
-   **`ncs_fundamentals_guide.md`**: **ACADEMY CORE**. DeviceTree, GPIO API, Kconfig/prj.conf, and Logging (from Nordic Academy Lessons 2-4).
-   **`ble_core_guide.md`**: **BLE Bible**. Concepts, Stack, Roles, and Production Guide.
-   **`jlink_debug_guide.md`**: **Debugging**. RTT, Commander, and Recovering Bricked Boards.
-   **`zephyr_basics.md`**: Quick reference for DeviceTree syntax, Kconfig, and Driver Model concepts.
-   **`ncs_project_structure.md`**: Where files belong (`boards/`, `child_image/`, `src/`).

## 3. The Implementation (Writing Code)
-   **`zephyr_peripherals.md`**: "Gold Standard" copy-paste patterns for GPIO, I2C, SPI.
-   **`nordic_docs_index.md`**: Links to official Hardware Datasheets (Product Specs).

## 4. Real-World Architectures & Case Studies
-   **`seeed_xiao_master_guide.md`**: **MASTER GUIDE**. Covers nRF52840 vs nRF54L15, Battery/UART traps, and Protocol capabilities.
-   **`xiao_ble_architecture.md`**: Deep dive into the nRF52840 variant's dual-core strategy.
-   **`meow_sense_tag_lessons.md`**: **HARD-WON LESSONS**. nRF5340 Dual-Core, BQ25120A PMIC, and rescue strategies.
-   **`real_world_architectures.md`**: Analysis of *OpenEarable*, *Seeed*, and *MakerDiary*. Comparison of module-based vs bare-chip designs.Source projects.

## 5. Meta-Skills & Agent Strategy
-   **`ai_embedded_skill_roadmap.md`**: **STRATEGY CORE**. The roadmap for checking hardware truth (SVD/Netlist) and using hierarchical verification agents.

## 6. Examples
-   **`examples/basic_blinky`**: The "Hello World" of NCS.
-   **`examples/gpio_button_led`**: Interrupt-based Button → LED (Nordic Academy Lesson 2).
-   **`examples/logging_demo`**: Zephyr Logger Module with all severity levels + hexdump (Nordic Academy Lesson 4).
-   **`examples/i2c_sensor_read`**: Reference implementation of the "Hardware First" workflow.
