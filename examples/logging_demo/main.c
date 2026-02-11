/*
 * Nordic Academy — NCS Fundamentals Lesson 4
 * Example: Zephyr Logging Module Demo
 *
 * Demonstrates:
 *   - LOG_MODULE_REGISTER with custom level
 *   - All 4 severity levels (ERR, WRN, INF, DBG)
 *   - LOG_HEXDUMP for raw data inspection
 *   - Button-triggered logging (interrupt-based)
 *
 * Board: Any NCS-supported board with sw0 alias
 * Build: west build -b <board> -p always
 */

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/* Register this module with the Logger at DEBUG level */
LOG_MODULE_REGISTER(logging_demo, LOG_LEVEL_DBG);

#define SW0_NODE DT_ALIAS(sw0)
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
static struct gpio_callback button_cb_data;

/* Sample data for hexdump */
static uint8_t sample_data[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x02, 0x03};

/* Factorial calculation for demonstration */
static long factorial(int n) {
  long result = 1;
  for (int i = 2; i <= n; i++) {
    result *= i;
  }
  return result;
}

/* Button ISR */
void button_handler(const struct device *dev, struct gpio_callback *cb,
                    uint32_t pins) {
  static int press_count = 0;
  press_count++;

  LOG_INF("Button pressed! Count: %d", press_count);
  LOG_DBG("Pin mask that triggered: 0x%08x", pins);

  /* Show factorial calculation */
  for (int i = 1; i <= 5; i++) {
    LOG_INF("Factorial of %d = %ld", i, factorial(i));
  }

  /* Demonstrate hexdump */
  LOG_HEXDUMP_INF(sample_data, sizeof(sample_data), "Sample Data:");

  /* Demonstrate warning and error (for learning purposes) */
  if (press_count > 5) {
    LOG_WRN("Button pressed %d times — consider debouncing.", press_count);
  }
  if (press_count > 10) {
    LOG_ERR("Excessive button presses detected!");
  }
}

int main(void) {
  int ret;

  LOG_INF("=== NCS Fundamentals — Logging Demo ===");
  LOG_INF("Log levels: ERR=1, WRN=2, INF=3, DBG=4");
  LOG_DBG("This message only appears at LOG_LEVEL_DBG");

  if (!gpio_is_ready_dt(&button)) {
    LOG_ERR("Button device not ready!");
    return -1;
  }

  ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
  if (ret < 0) {
    LOG_ERR("Failed to configure button: %d", ret);
    return -1;
  }

  ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
  if (ret < 0) {
    LOG_ERR("Failed to configure interrupt: %d", ret);
    return -1;
  }

  gpio_init_callback(&button_cb_data, button_handler, BIT(button.pin));
  gpio_add_callback(button.port, &button_cb_data);

  LOG_INF("Press the button to see log output.");

  while (1) {
    k_msleep(60000);
  }

  return 0;
}
