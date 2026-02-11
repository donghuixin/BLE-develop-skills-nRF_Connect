/*
 * Nordic Academy — NCS Fundamentals Lesson 4
 * Example: UART Async API — LED Control via Serial
 *
 * Demonstrates:
 *   - UART Asynchronous API with EasyDMA
 *   - Callback-driven RX/TX
 *   - Continuous reception with auto-re-enable
 *   - Press '1', '2', '3' over serial to toggle LEDs
 *
 * Board: Any NCS board with uart0, led0, led1, led2
 * Build: west build -b <board> -p always
 */

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>

#define RECEIVE_BUFF_SIZE 10
#define RECEIVE_TIMEOUT 100

/* LEDs from DeviceTree */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

/* UART buffers */
static uint8_t rx_buf[RECEIVE_BUFF_SIZE] = {0};
static uint8_t tx_buf[] = {"nRF Connect SDK Fundamentals - UART Demo\r\n"
                           "Press 1-3 to toggle LEDs\r\n"};

/* UART Async Callback */
static void uart_cb(const struct device *dev, struct uart_event *evt,
                    void *user_data) {
  switch (evt->type) {
  case UART_RX_RDY:
    if (evt->data.rx.len == 1) {
      uint8_t ch = evt->data.rx.buf[evt->data.rx.offset];
      if (ch == '1') {
        gpio_pin_toggle_dt(&led0);
      } else if (ch == '2') {
        gpio_pin_toggle_dt(&led1);
      } else if (ch == '3') {
        gpio_pin_toggle_dt(&led2);
      }
    }
    break;
  case UART_RX_DISABLED:
    /* Re-enable to keep listening */
    uart_rx_enable(dev, rx_buf, sizeof(rx_buf), RECEIVE_TIMEOUT);
    break;
  default:
    break;
  }
}

int main(void) {
  int ret;
  const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

  if (!device_is_ready(uart)) {
    return -1;
  }
  if (!gpio_is_ready_dt(&led0)) {
    return -1;
  }

  gpio_pin_configure_dt(&led0, GPIO_OUTPUT_INACTIVE);
  gpio_pin_configure_dt(&led1, GPIO_OUTPUT_INACTIVE);
  gpio_pin_configure_dt(&led2, GPIO_OUTPUT_INACTIVE);

  ret = uart_callback_set(uart, uart_cb, NULL);
  if (ret < 0) {
    return -1;
  }

  ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_US);
  if (ret < 0) {
    return -1;
  }

  ret = uart_rx_enable(uart, rx_buf, sizeof(rx_buf), RECEIVE_TIMEOUT);
  if (ret < 0) {
    return -1;
  }

  while (1) {
    k_sleep(K_FOREVER);
  }

  return 0;
}
