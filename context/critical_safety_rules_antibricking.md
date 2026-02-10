# Critical Safety & Anti-Bricking Rules (Read Before Coding!)

**Applicability**: These rules apply to **ALL** Nordic series (nRF52, nRF53, nRF91, nRF54) and any similar Cortex-M based embedded platforms with Flash-based configuration registers (UICR) or PMIC dependencies.

## 🚨 CARDINAL RULE: NO Runtime UICR Writes
**Risk**: Permanent "Bricking" (SWD Lockout).
-   **Universal Truth**: All Nordic chips use UICR to control `APPROTECT` (Access Port Protection).
-   **Restriction**: Do NOT write to User Information Configuration Registers (UICR) from user firmware.
-   **Reason**: Writing UICR requires NVMC HV modes. If VDD drops (e.g., PMIC reset) during this write, the `APPROTECT` bits can flip to "Locked".
-   **Correct Approach**: Configure UICR settings (like APPROTECT or REGOUT0) using **external tools only** (e.g., `nrfjprog --memwr` or a dedicated production script) *after* the firmware is stable.

## 🧱 The "Backup Poison" Trap (APPROTECT)
**Risk**: Restoring a "Locked" state after a chip erase.
-   **Scenario**: A fresh chip often has APPROTECT "Enabled" (Locked) by default. If you use `nrfjprog --readuicr` to backup the chip state, you are saving the "Lock" bit.
-   **Danger**: After a `--chiperase` (which unlocks the chip), if you restore that backup, you re-lock the chip. If the firmware doesn't immediately unlock it, the device becomes inaccessible via SWD.
-   **Rule**: Never blindly restore entire UICR backups. Always explicitly set APPROTECT to `0x50FA50FA` (Unlocked) during new firmware flashing.

## 🧩 Multi-Core Dependencies (nRF53/54)
**Risk**: Silent Failure / "Infinite Boot Loop".
-   **Context**: Dual-core chips (App Core + Net Core).
-   **Issue**: The default `bt_enable()` in Zephyr waits for the Network Core to respond via IPC.
-   **Requirement 1**: You MUST enable the auxiliary core in Kconfig (`CONFIG_SOC_NRF53_CPUNET_ENABLE=y`).
-   **Requirement 2**: You MUST flash firmware to **BOTH** cores. Flashing only the App Core (`merged.hex`) leaves the Net Core empty, causing the App Core to hang forever waiting for a response.

## 🔌 Power & Reset Hardware Dependencies
**Risk**: MCU appears dead but is actually held in Reset by PMIC.
-   **Context**: Designs with PMICs (e.g., BQ25120A) or Watchdogs controlling the `RESET` line.
-   **Issue**: If the MCU firmware fails to configure the PMIC's "Watchdog" or "Ship Mode" pins (e.g., CD pin) correctly on boot, the PMIC may cut power or pull `RESET` low.
-   **Rule**:
    1.  Verify the schematic: Which pin keeps the PMIC alive? (e.g., `P0.17` vs `P1.05`).
    2.  **Early Init**: Configure these "Keep-Alive" pins immediately in `main()` or using `gpio-hog` in DeviceTree to ensure they are set before the kernel fully boots.

## ⚡ Low Power & Debugging
-   **Issue**: Sleeping deeply (`k_sleep` or `WFI`) can sometimes disconnect the debugger.
-   **Debug Mode**: When `CONFIG_DEBUG=y` is set, ensure the power management policy prevents deep system off if you need sustained RTT logging.
