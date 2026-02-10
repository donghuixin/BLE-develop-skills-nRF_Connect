# Case Study: Meow Sense Tag (nRF5340) Lessons Learned

*Source: Real-world development of nRF5340 (MDBT53-V-DB-40) based tag.*

## 🚨 The Anti-Bricking "Golden Rules" (nRF5340 Specific)

### 1. Never Write UICR via Firmware
-   **Risk**: NVMC operations (erase/write) are high-power. Any voltage drop (PMIC reset, battery jitter) during UICR write can corrupt the register.
-   **Consequence**: If `APPROTECT` bits flip to an illegal state or lock unexpectedly -> **Permanent SWD Lockout**.
-   **Standard**: UICR writing belongs in the **Flash Script**, not the `.c` code.

### 2. The "Toxic" UICR Backup
-   **Danger**: Restoring a UICR backup read from a fresh chip (default `0xFFFFFFFF`) to a chip-erased chip will **Lock** it immediately upon reset.
-   **Fix**: A flash script must **unconditionally** overwrite the `APPROTECT` (0x00FF8000) and `SECUREAPPROTECT` (0x00FF800C) addresses with `0x50FA50FA` after restoring any backup.

### 3. Network Core Activation
-   **Symptom**: `bt_enable()` hangs forever.
-   **Cause**: Network core is in `FORCEOFF` by default.
-   **Fix**: `CONFIG_SOC_NRF53_CPUNET_ENABLE=y`. In code, ensure `NRF_RESET->NETWORK.FORCEOFF` is released.

## 🔌 Hardware & PMIC (BQ25120A) Lessons

### 1. The CD Pin Trap (P0.17 vs P1.05)
-   **Classic Mistake**: Confusing "ON+BTN" (P1.05) with "CD" (P0.17).
-   **Truth**: BQ25120A's `CD` pin MUST be high for the PMIC to exit Ship/Hi-Z mode.
-   **Startup**: Must pull P0.17 HIGH using raw registers in `SYS_INIT` (PRE_KERNEL_1) before the I2C driver is even ready.

### 2. The /RESET Feedback Loop
-   BQ25120A `/RESET` (D3) connects to nRF5340 `RESETN`.
-   If PMIC is in Hi-Z (CD=LOW) and no USB is connected, `/RESET` stays LOW, keeping the MCU in **continuous hardware reset**.
-   **Diagnosis**: Measure `RESETN`. If 0V, flywire `CD` (P0.17) to VDD to wake it up.

### 3. LDO 3.3V Enablement
-   Must write to register `0x07` (LS_LDO_CTRL) with `0xE4` to enable the 3.3V rail.
-   **Avoid Register 0x06**: It doesn't exist on BQ25120A. Writing to it causes undefined behavior/resets.

## 🛠 Debugging & Tooling Best Practices

### 1. The "Invisible" Flash Failure
-   VSCode Extension `settings.json` pointing to the wrong toolchain folder can cause "Successful" flashes that actually flash old/wrong hex files.
-   **Fix**: Use `tasks.json` to hardcode the `PATH` to the correct toolchain.

### 2. Verification Command
-   `nrfjprog --verify build/merged.hex` is the ONLY way to be 100% sure the hardware matches the build.
-   If verification passes but behavior doesn't change -> Check phone BLE cache (Unpair/Forget device).

### 3. PC/SP Inspection
-   `nrfjprog --readregs`
-   **PC ~ 0x000xxxxx**: Code is running in Flash.
-   **PC = 0x00000000**: MCU is halted or crashed at vector.

## 📝 Success Workflow Summary
1.  **Modify** Code/Kconfig.
2.  **Pristine Build**: `west build ... -p`.
3.  **Secure Flash**: Use script that handles NET core, APP core, and UICR Unlock.
4.  **Verify**: `nrfjprog --verify`.
5.  **Check Heartbeat**: Logs/LEDs.
