# NCS Fundamentals Guide
*Source: Nordic Developer Academy — nRF Connect SDK Fundamentals (Lessons 2-4)*

> [!IMPORTANT]
> This guide covers the **foundational APIs and patterns** for NCS development.
> Always verify pin mappings against the board's `.dts` file and schematic.

## 1. DeviceTree Essentials

### Structure
```
/dts-v1/;
/ {                            ← Root Node
    a-node {                   ← Child Node
        subnode_label: a-sub-node {  ← Node with Label
            foo = <3>;         ← Property (integer)
        };
    };
    aliases {
        led0 = &led0_label;   ← Alias → Label reference
    };
};
```

### Key Concepts
| Concept | Description |
| :--- | :--- |
| **Node** | Represents a hardware component |
| **Label** | Unique shorthand to reference a node (e.g., `led0`) |
| **Property** | Name/value pair on a node (string, int, boolean, bytes) |
| **Binding (YAML)** | Schema defining required properties for a `compatible` string |
| **Alias** | Maps generic name (`led0`) to specific label for portability |

### C Macros for Accessing DeviceTree
```c
DT_NODELABEL(label)           // Get node ID by label
DT_ALIAS(alias_name)          // Get node ID by alias
DT_PROP(node_id, prop_name)   // Get property value
```

### File Hierarchy (Example: nRF52833 DK)
```
nrf52833dk_nrf52833.dts        ← Board (LEDs, Buttons, Aliases)
  └── nrf52833dk-pinctrl.dtsi  ← Pin Control (UART/SPI pin mappings)
  └── nrf52833_qiaa.dtsi       ← SoC Variant (RAM/Flash sizes)
      └── nrf52833.dtsi        ← Base SoC (All peripherals)
```

> **nRF54 Note**: On nRF54 DKs, PCB silkscreen "LED0" matches `led0` alias.
> On older kits, "LED1" on PCB = `led0` in code.

## 2. GPIO Generic API

### The `gpio_dt_spec` Struct
Packages all DeviceTree info for a pin into one struct:
```c
#include <zephyr/drivers/gpio.h>

// Get spec from DeviceTree alias
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
```

### API Cheat Sheet
| Function | Purpose |
| :--- | :--- |
| `gpio_is_ready_dt(&spec)` | Check if GPIO port is ready |
| `gpio_pin_configure_dt(&spec, flags)` | Configure pin (INPUT/OUTPUT) |
| `gpio_pin_set_dt(&spec, value)` | Set output (1=active, 0=inactive) |
| `gpio_pin_toggle_dt(&spec)` | Toggle output state |
| `gpio_pin_get_dt(&spec)` | Read input (polling) |
| `gpio_pin_interrupt_configure_dt(&spec, flags)` | Enable interrupt |
| `gpio_init_callback(&cb, handler, BIT(pin))` | Init callback struct |
| `gpio_add_callback(port, &cb)` | Register callback |

### Interrupt Pattern (Recommended over Polling)
```c
static struct gpio_callback button_cb_data;

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    gpio_pin_toggle_dt(&led);
}

// In main():
gpio_pin_interrupt_configure_dt(&btn, GPIO_INT_EDGE_TO_ACTIVE);
gpio_init_callback(&button_cb_data, button_pressed, BIT(btn.pin));
gpio_add_callback(btn.port, &button_cb_data);
```

## 3. Kconfig & Configuration

### prj.conf Rules
- Syntax: `CONFIG_<SYMBOL>=<value>` — **NO spaces** around `=`.
- `prj.conf` **overrides** board defconfig (`<board>_defconfig`).
- Never modify board defconfig directly.

### Configuration File Types
| File | Purpose |
| :--- | :--- |
| `prj.conf` | Application feature flags |
| `app.overlay` | Hardware description overrides |
| `CMakeLists.txt` | Build system control |
| `sysbuild.conf` | Multi-image build config |
| `<board>.overlay` | Auto-included if name matches board |

### Build Target Security (nRF53/nRF91)
- `<board>`: Single image, full privileges.
- `<board>/ns`: Non-Secure image + TF-M (TrustZone).

> **Deprecation**: Child/Parent image system replaced by **Sysbuild** in NCS v2.8.0+.

## 4. Logging

### printk() — Simple but Blocking
```c
#include <zephyr/sys/printk.h>
printk("Value: %d\n", value);  // Blocks until sent
```

### Logger Module — Recommended (Async, Filtered)
```c
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(my_module, LOG_LEVEL_DBG);

LOG_INF("Info message: %d", val);
LOG_WRN("Warning!");
LOG_ERR("Error!");
LOG_DBG("Debug only");
LOG_HEXDUMP_INF(data, sizeof(data), "Raw Data:");
```

### Kconfig for Logging
```conf
CONFIG_LOG=y
CONFIG_LOG_DEFAULT_LEVEL=3       # 1=ERR, 2=WRN, 3=INF, 4=DBG
CONFIG_LOG_BACKEND_SHOW_COLOR=y  # Color: Red=ERR, Yellow=WRN
```

### Multi-file Modules
```c
// file1.c
LOG_MODULE_REGISTER(my_module, LOG_LEVEL_INF);

// file2.c  
LOG_MODULE_DECLARE(my_module);  // Share the same module
```
