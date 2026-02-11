/*
 * Nordic Academy — NCS Fundamentals Lesson 2
 * Example: GPIO Button → LED (Interrupt-based)
 *
 * This example demonstrates the recommended interrupt pattern:
 *   - Button press triggers an ISR callback
 *   - Callback toggles an LED
 *   - Main thread sleeps (power-efficient)
 *
 * Board: Any NCS-supported board with led0 and sw0 aliases
 * Build: west build -b <board> -p always
 */

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

/* Sleep time for the main thread (long, since interrupts handle work) */
#define SLEEP_TIME_MS (10 * 60 * 1000) /* 10 minutes */

/* Get hardware specs from DeviceTree aliases */
#define LED0_NODE DT_ALIAS(led0)
#define SW0_NODE DT_ALIAS(sw0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

/* Callback data structure — must be static */
static struct gpio_callback button_cb_data;

/* ISR: Called on button press edge */
void button_pressed(const struct device *dev, struct gpio_callback *cb,
                    uint32_t pins) {
  gpio_pin_toggle_dt(&led);
}

int main(void) {
  int ret;

  /* 1. Verify devices are ready */
  if (!gpio_is_ready_dt(&led)) {
    return -1;
  }
  if (!gpio_is_ready_dt(&button)) {
    return -1;
  }

  /* 2. Configure LED as output, Button as input */
  ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
  if (ret < 0) {
    return -1;
  }
  ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
  if (ret < 0) {
    return -1;
  }

  /* 3. Configure interrupt on button (edge-triggered) */
  ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
  if (ret < 0) {
    return -1;
  }

  /* 4. Initialize and register callback */
  gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
  gpio_add_callback(button.port, &button_cb_data);

  /* 5. Main thread sleeps — interrupts do all the work */
  while (1) {
    k_msleep(SLEEP_TIME_MS);
  }

  return 0;
}
