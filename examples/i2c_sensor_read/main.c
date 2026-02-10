/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/*
 * 1. Retrieve the DeviceTree Node
 * Rule: Hardware is defined in Overlay, NOT in C code.
 * Matches "temp_sensor" label in app.overlay
 */
#define SENSOR_NODE DT_NODELABEL(temp_sensor)

/*
 * 2. usage Check: Is the node enabled?
 */
#if !DT_NODE_HAS_STATUS(SENSOR_NODE, okay)
#error "Unsupported board: temp_sensor devicetree node is not defined"
#endif

/*
 * 3. Define the I2C Specification
 * This struct holds the bus pointer and the address from DTS.
 * Rule: Use _dt APIs to respect the Devicetree configuration.
 */
static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(SENSOR_NODE);

int main(void) {
  int ret;
  uint8_t config[2] = {0x01, 0x80}; // Example: Config Register, Value
  uint8_t data[2];

  /* 4. Runtime Safety Check */
  if (!device_is_ready(dev_i2c.bus)) {
    printk("I2C bus %s is not ready!\n", dev_i2c.bus->name);
    return 0;
  }

  printk("Found I2C device at addr 0x%02X\n", dev_i2c.addr);

  /* 5. Protocol Operations using _dt API */

  // Write Configuration
  ret = i2c_write_dt(&dev_i2c, config, sizeof(config));
  if (ret != 0) {
    printk("Failed to write to I2C device\n");
    return 0;
  }

  while (1) {
    k_sleep(K_MSEC(1000));

    // Read Temperature (Example 2-byte read)
    ret = i2c_read_dt(&dev_i2c, data, sizeof(data));
    if (ret != 0) {
      printk("Failed to read from I2C device\n");
      continue;
    }

    printk("Raw Data: 0x%02X 0x%02X\n", data[0], data[1]);
  }
  return 0;
}
