# CLAUDE.md

This file provides high-context guidance for AI assistants (Claude, ChatGPT, Cursor) working in this nRF Connect SDK (NCS) repository.

## 🚨 CRITICAL SAFETY RULES (READ FIRST)
**You MUST STOP and warn the user if a request involves:**
1.  **Writing UICR**: `NRF_UICR->...` or `nrf_nvmc_write_uicr` at runtime. **(Risk: PERMANENT BRICK)**.
2.  **PMIC/Regulators**: Changing `regulator-initial-mode` or writing to BQ25120A register `0x06` (non-existent).
3.  **Network Core/Secure Boot**: Modifying `child_image/` or `NRF_RESET->NETWORK.FORCEOFF` without checking `CONFIG_SOC_NRF53_CPUNET_ENABLE`.
4.  **IO Confusion**: Swapping **P0.17** (PMIC CD) with **P1.05** (Button) on nRF5340 custom boards.
5.  **Leakage Control**: Modifying **P0.14** on XIAO BLE (Sense) without accounting for the battery voltage divider switch.
6.  **nRF54L15 Boot**: Deploying nRF54L15 on battery with `CONFIG_SERIAL=y` (Code will hang waiting for USB).

---

## 🛠 Build & Environment (West)
-   **Initialize**: `west init -m <url> --mr <revision>` (Only for new workspace)
-   **Update Deps**: `west update` (Run often to sync `sdk-nrf` and modules)
-   **Build**: `west build -b <board_name> -d build`
    -   *Pristine*: `west build -p always ...` (Use when Kconfig/DTS changes don't pick up)
-   **Flash**: `west flash`
-   **Clean**: `rm -rf build`

## 🏗 Architecture Rules (Strict)
1.  **Hardware First**:
    -   Never hardcode pins in C. Define them in `app.overlay` or `boards/*.overlay`.
    -   Use `DT_ALIAS()` or `DT_NODELABEL()` to access hardware.
2.  **Config Separation**:
    -   Software features (Bluetooth, Logging) -> `prj.conf`.
    -   Hardware definitions (pins, sensors) -> `app.overlay`.
3.  **App Structure**:
    -   Avoid "Mega-Main". Split logic into `src/managers/` or `src/modules/` (e.g., `ble_manager.c`, `sensor_manager.c`).
    -   For nRF53/91: Use `child_image/` for Network Core/Bootloader config.

## 📜 Code Style (Zephyr/Linux)
-   **Standard**: C99/C11.
-   **Naming**: `snake_case` for variables/functions. `SCREAMING_SNAKE` for macros.
-   **Error Handling**:
    -   Check `int ret < 0` for **ALL** API calls.
    -   Use `device_is_ready()` for **ALL** device pointers.
-   **Concurrency**:
    -   Use `k_sleep()` (yields), NOT `k_busy_wait()` (blocks).
    -   ISR -> Semaphore -> Thread for processing data.

## 📚 Knowledge Base
-   `context/ncs_knowledge_map.md`: **Start Here** for documentation index.
-   `context/ncs_engineering_standards.md`: The "Constitution" of this repo.
