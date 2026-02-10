# Zephyr & NCS Context for AI

## Key Concepts

### 1. DeviceTree (DTS)
- **Role**: Describes hardware. **Immutable** at runtime.
- **Syntax**: `.dts` (board), `.overlay` (application).
- **Access**: Use `DT_*` macros (e.g., `DT_ALIAS(led0)`, `DT_NODELABEL(i2c0)`).
- **Important**: Hardware setup (pinmux, interrupts) is done in DTS, *not* in C code.

### 2. Kconfig
- **Role**: Configures software features (kernel size, drivers, stacks).
- **Syntax**: `CONFIG_XYZ=y` in `prj.conf`.
- **Dependency**: Enabling a driver in Kconfig often requires a corresponding node in DTS with `status = "okay"`.

### 3. Driver Model
- **Pattern**: `const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(my_dev));`
- **Check**: `if (!device_is_ready(dev)) { /* Error */ }`
- **Unified API**: `sensor_sample_fetch`, `gpio_pin_set_dt`, `uart_tx`.

### 4. Threading
- **Priorities**: Cooperative (negative) vs Preemptible (non-negative).
- **Sleeping**: `k_sleep(K_MSEC(100))` yields CPU. `k_busy_wait` does *not*.

### 5. Work queues
- Use system workqueue for deferring ISR work: `k_work_submit()`.
- Essential for operations that can sleep (I2C/SPI) which cannot run in ISR.
