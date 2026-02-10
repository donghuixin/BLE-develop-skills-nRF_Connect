# NCS Engineering Standards & Philosophy

*Source: Engineering Handbook*

## 1. The Paradigm Shift: Bare Metal to RTOS
All new projects must recognize the fundamental shift from nRF5 SDK to nRF Connect SDK (NCS).
-   **Core Philosophy**: "Decouple and Reuse". Hardware details (`.dts`) are strictly separated from Application Logic (`.c`).
-   **The OS**: Zephyr RTOS is the foundation. It is not optional. It provides the scheduler, drivers, and consistency across nRF52/53/54.

### Comparison: Old vs New

| Feature | nRF5 SDK (Legacy) | nRF Connect SDK (Standard) |
| :--- | :--- | :--- |
| **Core** | Bare Metal / Scheduler | **Zephyr RTOS** |
| **Build** | Makefile / IDE | **West (CMake + Ninja)** |
| **Hardware** | C Headers | **Devicetree (.dts)** |
| **Config** | `sdk_config.h` | **Kconfig (`prj.conf`)** |
| **Repos** | Zip / Single Repo | **Multi-Repo (West Managed)** |

## 2. The West Ecosystem
NCS is not a single repo; it is a "Workspace" managed by `west`.

### The 4 Pillars of a Workspace
1.  **`sdk-nrf`**: Nordic's core repo (Applications, Samples, Wireless Protocols).
2.  **`nrfxlib`**: Closed-source binaries (SoftDevice Controller, Crypto accelerators).
3.  **`zephyr`**: The RTOS kernel, OS services, and generic drivers.
4.  **`mcuboot`**: Secure Bootloader.

### Essential Commands
-   `west update`: Syncs all git repos in the workspace to the exact versions defined in the manifest. **Run this often.**
-   `west build`: Compiles the project.
    -   *Under the hood*: Calls CMake -> Generates Devicetree headers -> Calls Ninja -> Calls GCC.
-   `west flash`: Programs the board.

## 3. Engineering Best Practices

### A. Decoupling (The Golden Rule)
-   **Hardware**: Defined **ONLY** in `*.overlay` or `*.dts`.
    -   *Example*: Adding a button? Do it in Overlay. Do NOT write C code to configure P0.05 mapping.
-   **Software Features**: Configured **ONLY** in `prj.conf` (Kconfig).
    -   *Example*: Need Bluetooth? set `CONFIG_BT=y`. The build system handles the rest (linking libraries, enabling stack).

### B. Threading Model
Don't use `main()` for everything. Structure app into threads based on priority:

| Type | Context | Priority | Use Case |
| :--- | :--- | :--- | :--- |
| **ISR** | Interrupt | Highest | **Short** hardware events (GPIO, DMA). No blocking allowed. |
| **Coop Thread** | Thread | High | Critical tasks (cannot be preempted by other threads). |
| **Main/User** | Thread | Medium | General logic (Sensor processing, State machine). |
| **System** | Thread | Low | Logging, Idle, Workqueue. |

### C. Synchronization
Race conditions are the enemy. Use the right tool:
-   **Semaphores**: Signaling (e.g., ISR tells Thread "Data is ready").
-   **Mutexes**: Protection (e.g., Two threads want to use the same UART). **Mandatory** for shared resources.
-   **Message Queues**: Data passing. Thread-safe buffer between Producer and Consumer.

## 4. Modern Architecture (nRF53/54)
-   **nRF54L Support**: Uses **GRTC** (Global RTC) instead of RTC1. The SDK abstracts this.
-   **Sysbuild**: The new build coordinator. Replaces "Parent-Child" images. Mandatory for complex multi-core builds (e.g., App + Net + Bootloader).
