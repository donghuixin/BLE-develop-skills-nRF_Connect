# J-Link Debugging & RTT Guide

*Source: SEGGER J-Link / J-Trace User Guide*

## 1. RTT (Real Time Transfer)
RTT is the "Secret Weapon" of Nordic development. It allows high-speed logging without using a UART peripheral.

### How it works
-   Uses a control block in RAM.
-   J-Link reads RAM in the background while CPU is running.
-   **Zero Latency**: Unlike UART, it doesn't block the CPU significantly.

### Enabling in NCS
In `prj.conf`:
```properties
CONFIG_USE_SEGGER_RTT=y
CONFIG_LOG_BACKEND_RTT=y
CONFIG_LOG_BACKEND_UART=n  # Optional: Disable UART to save power/pins
```

### Viewing Logs
-   **VS Code**: Use "nRF Terminal" -> Select "RTT".
-   **CLI**: `jlinkrttlogger` or via J-Link Commander.

## 2. J-Link Commander (`JLinkExe`)
The "Swiss Army Knife" for low-level hardware access. Essential when the board is "bricked" or locked.

### Common Commands
| Command | Description |
| :--- | :--- |
| `connect` | Connect to the target. |
| `r` | Reset the target. |
| `go` | Start CPU execution. |
| `h` | Halt CPU execution. |
| `w4 <addr> <val>` | Write 32-bit value to address. |
| `mem <addr> <len>` | Dump memory. |
| `loadfile <path>` | Load a hex/bin file manually. |
| `unlock kinetis` | (Not for Nordic) But `unlock` helps in some generic cases. |
| `erase` | Mass erase the chip. |

### Recovering a Locked nRF
If `nrfjprog --recover` fails, try J-Link Commander:
1.  `JLinkExe -device nrf52840_xxaa -if swd -speed 4000`
2.  `connect`
3.  `erase` (May require unlocking first).

## 3. J-Link Scripting (`.JLinkScript`)
Used for custom board setups, e.g., enabling power to the CPU via a PMIC *before* the debug session starts.

**Example Structure**:
```c
void InitTarget(void) {
  // Write to a register to enable power
  JLINK_MEM_WriteU32(0x50000504, 0x1);
}
```
To use: passing `-JLinkScriptFile path/to/script.jlink` to the runner.

## 4. Multi-Core Debugging (nRF53/54)
Debugging dual-core chips requires keeping track of **AP (Access Ports)**.

-   **Application Core**: Usually on the main AP.
-   **Network Core**: Often on a separate AP or requires a different J-Link Selection.

**VS Code Tip**: Use `cal101/nrf-connect-extension`. It automatically sets up `launch.json` to attach to the correct core.

## 5. Troubleshooting
-   **VRef (VTref)**: Must detect ~3.0V (or board voltage). If 0V, J-Link won't drive SWD.
-   **Speed**: Default is often 4000kHz. If cables are long or signal is noisy, lower to 1000kHz (`speed 1000`).
-   **Low Power**: If the device enters `System OFF`, the debug interface dies.
    -   *Fix*: Wake it up or hold Reset while connecting.

## 6. J-Link vs nRF Connect
-   **J-Link**: The underlying driver/hardware.
-   **nrfjprog**: Nordic's command-line tool that wraps J-Link DLLs.
-   **West**: Calls `nrfjprog` or `jlink` runner.

**Recommendation**: Use `west flash` for daily work. Drop to `JLinkExe` only for "Unbricking" or deep hardware inspection.
