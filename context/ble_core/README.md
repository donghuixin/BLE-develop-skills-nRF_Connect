# BLE Core 5.0 Knowledge Base

This section contains a structured breakdown of the Bluetooth Low Energy (BLE) Core Specification 5.0.

## Modules

### 1. [Architecture Overview](01_architecture.md)
*   High-level view of the Controller, Host, and Application layers.
*   Explanation of the Host Controller Interface (HCI).
*   Summary of BLE 5.0 enhancements.

### 2. [Link Layer (LL)](02_link_layer.md)
*   Detailed breakdown of the radio states (Standby, Adv, Scan, Init, Conn).
*   Explanation of Roles (Master/Slave).
*   Deep dive into Air Interface, Channels, and 5.0 features (Advertiser Extensions, Periodic Adv, Coded PHY).

### 3. [Host Protocols](03_host_protocols.md)
*   **L2CAP**: Multiplexing and Segmentation.
*   **ATT & GATT**: Data model, Services, and Characteristics.
*   **GAP**: Device discovery and connection modes.
*   **SM**: Security, Pairing, and Encryption.

## Recommended Reading Order
1.  Start with **Architecture** to understand the big picture.
2.  Dive into **Link Layer** to understand how devices communicate over the air.
3.  Finish with **Host Protocols** to understand how applications exchange data.
