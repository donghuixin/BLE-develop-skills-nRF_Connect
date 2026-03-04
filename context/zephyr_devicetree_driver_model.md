# Zephyr DeviceTree 与驱动模型详解

**原始博客链接**: [详解Zephyr设备树(DeviceTree)与驱动模型](https://www.nordicsemi.cn/blog/zephyr-devicetree-and-driving-model/)

## 1. 核心思想与硬件抽象解耦
Zephyr 最大的优势之一是丰富的生态（开源协议栈、框架、驱动等）。基于此，Zephyr采用 **Kconfig** 来管理系统配置，采用 **DeviceTree (设备树)** 描述硬件连接，以实现驱动代码与底层硬件描述的完全解耦。

传统方法中，开发者往往通过宏定义（如 `#define PIN_LED GPIO_PIN_12`）来抽象GPIO引脚。当发生板级更改时仍需要逐个验证宏。
而在 Zephyr 中，各半导体厂商（如 Nordic）按规范提供 DeviceTree 和底层驱动。开发者只需要修改并完善 DeviceTree，然后在应用程序中直接调用宏和跨平台API，驱动程序将在 application_main 执行前自动完成对应硬件的加载和初始化。

## 2. 在 C 代码中提取 DeviceTree 信息
DeviceTree 的配置文件 (`.dts` 及 `.overlay`) 在项目编译时最终会被转换为 `devicetree_generated.h`，该文件包含了描述的所有硬件节点属性。

- **命名转换规则**：为了在 C 语言中合法使用，Zephyr 强制将所有节点名称、属性名称中的字母变**小写**，并将特殊符号 `-`, `,` 等转成**下划线** `_`（例：`zephyr,user` 会被转换为 `zephyr_user`，`test-gpios` 转换为 `test_gpios`）。
- **API引入**：要在 C 代码中访问这棵树，需 `#include <zephyr/devicetree.h>`。
- **获取和应用属性**：
  ```c
  #include <zephyr/drivers/gpio.h>
  
  // 1. 获取节点 ID
  #define NODE_ID DT_PATH(zephyr_user)
  
  // 2. 根据节点 ID 和 属性名称获取具体配置 (specifier)
  static const struct gpio_dt_spec test_io = GPIO_DT_SPEC_GET(NODE_ID, test_gpios);
  
  int main(void) {
      // 3. 检查设备有没有在底层初始化完毕
      if (!device_is_ready(test_io.port)) {
          return 0;
      }
      
      // 4. 操作设备的通用 API
      gpio_pin_configure_dt(&test_io, GPIO_OUTPUT_INACTIVE);
      gpio_pin_set_dt(&test_io, 1);
  }
  ```

## 3. 驱动程序是如何被绑定的
Zepyhr中的底层硬件被封装为包含多种配置、接口函数指针及运行状态的 `device` 结构体。

- **获取 Device 结构体指针**：
  1. **以名字查找**：纯软件驱动或者脱离DeviceTree的设备可以通过 `device_get_binding("My_Device_Name")`。
  2. **以 DeviceTree 查找**：底层驱动通过 `DEVICE_DT_DEFINE()` 初始化生成驱动结构体并与DTS节点绑定，应用层通过 `DEVICE_DT_GET(node_id)` 进行获取。
- **Kconfig 与 DeviceTree 双管齐下**：
  在实际项目中，要驱动某个外设起效，必须完成两件事，缺一不可：
  1. 在 `prj.conf` 中启用相应驱动配置选项 (如 `CONFIG_COUNTER=y`)。这使得 CMake 在编译时将厂商提供的底层驱动 C 源码编入固件。
  2. 在设备树或 overlay 文件中设该节点为 `status = "okay"`。这使得驱动代码内的遍历宏能够发现该节点，并在后台为其动态生成 `device` 结构体用于被应用调用。

## 4. 总结
Zephyr 的驱动模型极大地降低了应用程序跨平台和换片的难度。修改 DeviceTree 配置即可完成引脚切换或通道映射，驱动程序也会根据Kconfig和DTS的标记智能地进行按需编译和初始化加载，确保固件不臃肿。
