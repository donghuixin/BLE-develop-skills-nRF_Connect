# Zephyr Peripheral Drivers & Protocols

**Golden Rule**: If it's not in the DeviceTree, it doesn't exist. Never hardcode pin numbers in C.

## 1. GPIO (General Purpose IO)
-   **DeviceTree**:
    ```dts
    /* app.overlay */
    / {
        aliases {
            my-led = &led0;
            my-button = &button0;
        };
    };
    ```
-   **C API (@ref `zephyr/drivers/gpio.h`)**:
    ```c
    static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(my_led), gpios);
    
    if (!gpio_is_ready_dt(&led)) return -1;
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    gpio_pin_toggle_dt(&led);
    ```

## 2. I2C (Inter-Integrated Circuit)
-   **DeviceTree**:
    ```dts
    /* app.overlay */
    &i2c0 {
        status = "okay";
        pinctrl-0 = <&i2c0_default>; // Ensure pinctrl is defined if custom board
        
        my_sensor: sensor@4a {
            compatible = "i2c-device";
            reg = <0x4a>;
            label = "MY_SENSOR";
        };
    };
    ```
-   **C API (@ref `zephyr/drivers/i2c.h`)**:
    ```c
    #define SENSOR_NODE DT_NODELABEL(my_sensor)
    static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(SENSOR_NODE);

    if (!device_is_ready(dev_i2c.bus)) return -1;
    
    uint8_t config[2] = {REG_ADDR, VALUE};
    i2c_write_dt(&dev_i2c, config, sizeof(config));
    i2c_read_dt(&dev_i2c, buffer, len);
    ```

## 3. SPI (Serial Peripheral Interface)
-   **DeviceTree**:
    ```dts
    /* app.overlay */
    &spi1 {
        status = "okay";
        cs-gpios = <&gpio0 10 GPIO_ACTIVE_LOW>;
        
        my_spi_dev: device@0 {
            compatible = "spi-device";
            reg = <0>; // Chip Select index
            spi-max-frequency = <1000000>;
        };
    };
    ```
-   **C API (@ref `zephyr/drivers/spi.h`)**:
    ```c
    static const struct spi_dt_spec spi_dev = SPI_DT_SPEC_GET(DT_NODELABEL(my_spi_dev), spi_operation_flags, 0);
    
    struct spi_buf tx_buf = { .buf = tx_data, .len = sizeof(tx_data) };
    struct spi_buf_set tx = { .buffers = &tx_buf, .count = 1 };
    
    spi_transceive_dt(&spi_dev, &tx, &rx);
    ```

## 4. Common Pitfalls
-   **I2C Address**: The `reg` property in DTS is the 7-bit address. Do NOT shift it left by 1.
-   **SPI CS**: Define `cs-gpios` in the controller node, and `reg` as the index (0, 1, 2...) in the device node.
