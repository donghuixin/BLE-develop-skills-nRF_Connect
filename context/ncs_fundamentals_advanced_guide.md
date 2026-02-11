# NCS Fundamentals Advanced Guide
*Source: Nordic Developer Academy — nRF Connect SDK Fundamentals (Lessons 4, 6, 7, 8)*

> [!IMPORTANT]
> Pin assignments in UART/I2C overlays **MUST** match your specific board schematic.
> nRF52 pin numbers ≠ nRF54 pin numbers. Verify `.dts` before flashing.

## 1. UART (Lesson 4)

### Three API Modes
| Mode | Key Function | Blocking? | Best For |
| :--- | :--- | :--- | :--- |
| **Polling** | `uart_poll_in/out` | Yes (out) | Simple debug |
| **Interrupt** | `uart_irq_callback_*` | No | Event-driven |
| **Async (DMA)** | `uart_rx_enable`, `uart_tx` | No | **Production** |

### Async API Pattern (Recommended)
```c
#include <zephyr/drivers/uart.h>

const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

static uint8_t rx_buf[10];

static void uart_cb(const struct device *dev, struct uart_event *evt, void *data) {
    switch (evt->type) {
    case UART_RX_RDY:
        /* Process: evt->data.rx.buf[evt->data.rx.offset] */
        break;
    case UART_RX_DISABLED:
        uart_rx_enable(dev, rx_buf, sizeof(rx_buf), 100); /* Re-enable */
        break;
    default: break;
    }
}

/* In main(): */
uart_callback_set(uart, uart_cb, NULL);
uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_US);
uart_rx_enable(uart, rx_buf, sizeof(rx_buf), 100);
```

### Kconfig
```conf
CONFIG_SERIAL=y
CONFIG_UART_ASYNC_API=y
```

### Runtime Reconfiguration
```c
const struct uart_config cfg = {
    .baudrate = 115200,
    .parity = UART_CFG_PARITY_NONE,
    .stop_bits = UART_CFG_STOP_BITS_1,
    .data_bits = UART_CFG_DATA_BITS_8,
    .flow_ctrl = UART_CFG_FLOW_CTRL_NONE,
};
uart_configure(uart, &cfg);
```

---

## 2. I2C (Lesson 6)

### API Cheat Sheet
```c
#include <zephyr/drivers/i2c.h>

#define I2C_NODE DT_NODELABEL(mysensor)
static const struct i2c_dt_spec dev = I2C_DT_SPEC_GET(I2C_NODE);
```

| Function | Purpose |
| :--- | :--- |
| `i2c_write_dt(&dev, buf, len)` | Write bytes |
| `i2c_read_dt(&dev, buf, len)` | Read bytes |
| `i2c_write_read_dt(&dev, wr, wr_len, rd, rd_len)` | Write then Read (register access) |
| `i2c_burst_read_dt(&dev, start_reg, buf, len)` | Read sequential registers |
| `i2c_reg_read_byte_dt(&dev, reg, &val)` | Read single register byte |
| `i2c_reg_write_byte_dt(&dev, reg, val)` | Write single register byte |

### DeviceTree Overlay
```dts
&i2c0 {
    status = "okay";
    mysensor: mysensor@4a {
        compatible = "i2c-device";
        reg = < 0x4a >;
    };
};
```

### Kconfig
```conf
CONFIG_I2C=y
CONFIG_CBPRINTF_FP_SUPPORT=y  /* For float printing */
```

---

## 3. Multithreading (Lesson 7)

### Scheduler Rules
| Concept | Detail |
| :--- | :--- |
| **Cooperative** | Negative priority. Runs until it yields. |
| **Preemptive** | Priority ≥ 0. Can be preempted. |
| **Lower number = Higher priority** | Priority 4 > Priority 7 |
| **Main thread** | Priority 0 (default) |
| **Idle thread** | Priority 15 (lowest) |
| **ISRs** | Preempt ALL threads. Keep short! |

### Thread Creation
```c
#define STACKSIZE 1024
#define PRIORITY  7

void my_thread(void) {
    while (1) {
        /* work */
        k_msleep(100);  /* Yield properly */
    }
}

K_THREAD_DEFINE(my_id, STACKSIZE, my_thread, NULL, NULL, NULL, PRIORITY, 0, 0);
```

### Critical: `k_msleep()` vs `k_busy_wait()`
- `k_msleep(ms)` → **Yields** CPU (good, power-efficient)
- `k_busy_wait(us)` → **Burns** CPU cycles (starves other threads!)

### Time Slicing (for equal-priority threads using busy-wait)
```conf
CONFIG_TIMESLICING=y
CONFIG_TIMESLICE_SIZE=10
CONFIG_TIMESLICE_PRIORITY=0
```

### Workqueues (Lighter than Threads)
```c
K_THREAD_STACK_DEFINE(wq_stack, 1024);
struct k_work_q my_wq;
struct k_work my_work;

void work_handler(struct k_work *work) { /* do stuff */ }

/* In main(): */
k_work_queue_start(&my_wq, wq_stack, K_THREAD_STACK_SIZEOF(wq_stack), 4, NULL);
k_work_init(&my_work, work_handler);
k_work_submit_to_queue(&my_wq, &my_work);
```

---

## 4. Thread Synchronization (Lesson 8)

### Semaphore vs Mutex
| Feature | Semaphore | Mutex |
| :--- | :--- | :--- |
| **State** | Count (0..N) | Locked / Unlocked |
| **Ownership** | None (any can give) | Owner only unlocks |
| **Priority Inheritance** | No | **Yes** |
| **ISR Safe** | Give only | **No** |
| **Reentrant** | No | Yes |

### Semaphore API
```c
K_SEM_DEFINE(my_sem, 0, 1);          /* Binary: init=0, max=1 */

k_sem_give(&my_sem);                  /* Signal (ISR-safe) */
k_sem_take(&my_sem, K_FOREVER);       /* Wait */
k_sem_count_get(&my_sem);             /* Check count */
```

### Mutex API
```c
K_MUTEX_DEFINE(my_mutex);

k_mutex_lock(&my_mutex, K_FOREVER);   /* Enter critical section */
/* ... protected code ... */
k_mutex_unlock(&my_mutex);            /* Exit critical section */
```

### Pattern: ISR → Semaphore → Thread
```c
K_SEM_DEFINE(data_ready, 0, 1);

void isr_handler(...) {
    k_sem_give(&data_ready);  /* Signal from ISR */
}

void processing_thread(void) {
    while (1) {
        k_sem_take(&data_ready, K_FOREVER);  /* Wait for signal */
        /* Process data safely in thread context */
    }
}
```
