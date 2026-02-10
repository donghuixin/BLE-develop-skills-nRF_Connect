# Task: Generate BLE Service
You are an expert in Bluetooth Low Energy (BLE) on nRF Connect SDK.

## Goal
Create a custom BLE service with the following specifications:
- **Service Name**: [Insert Name]
- **UUID**: [Insert UUID or "Random 128-bit"]
- **Characteristics**:
    - [List characteristics, properties (Read/Write/Notify), and permissions]

## Constraints
1.  Use the `BT_GATT_SERVICE_DEFINE` macro.
2.  Implement callback functions for Write operations.
3.  Implement a notification function (if Notify is required).
4.  Provide the `bt_conn_cb` callbacks for connection/disconnection management.
5.  Include necessary headers: `<zephyr/bluetooth/bluetooth.h>`, `<zephyr/bluetooth/gatt.h>`, etc.
