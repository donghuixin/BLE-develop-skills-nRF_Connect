# BLE Link Layer (LL)

The Link Layer is the control center of the BLE radio. It manages the state of the radio, scheduling, and protocol timing.

## 1. Link Layer State Machine
The Link Layer operates in one of the following states (BLE 5.0):

1.  **Standby**: The default state. The device is doing nothing (radio is off).
2.  **Advertising**: The device is transmitting advertising packets on the primary advertising channels (37, 38, 39).
3.  **Scanning**: The device is listening for advertising packets.
    *   **Passive Scanning**: Just listening.
    *   **Active Scanning**: Listening and requesting more info (Scan Request).
4.  **Initiating**: The device is listening for a specific advertiser to connect to it.
5.  **Connection**: The device is in a connection (Master or Slave).
6.  **Synchronization** (New in 5.0): The device is listening to a **Periodic Advertising** train.

> [!NOTE]
> **Isochronous Broadcasting** was introduced in **BLE 5.2** for LE Audio.

## 2. Roles
*   **Advertiser**: A device in the Advertising State.
*   **Scanner**: A device in the Scanning State.
*   **Initiator**: A device in the Initiating State.
*   **Master (Central)**: The device that initiated the connection. It controls the timing (Anchor Point).
*   **Slave (Peripheral)**: The device that accepted the connection. It follows the Master's timing.

## 3. Air Interface & Channels
BLE operates in the 2.4 GHz ISM band (2402–2480 MHz), divided into 40 channels of 2 MHz spacing.

*   **Primary Advertising Channels (3)**: Ch 37, 38, 39. Used for device discovery and connection setup. Avoids Wi-Fi channels 1, 6, 11 to reduce interference.
*   **Data Channels (37)**: Ch 0-36. Used for data transmission during a connection and for **Secondary Advertising** (BLE 5.0).

### 3.1 Adaptive Frequency Hopping (AFH)
The Link Layer re-maps the data channels to avoid noisy frequencies. The Master maintains a channel map and shares it with the Slave.

## 4. BLE 5.0 Key Link Layer Features

### 4.1 Advertising Extensions
Allows offloading advertising data from the 3 crowded primary channels to the 37 data channels.
*   **Primary Channel**: Sends a small pointer packet ("Go to Ch X at Time Y").
*   **Secondary Channel**: Sends the large payload (up to 255 bytes).

### 4.2 Periodic Advertising (Sync)
Allows a scanner to synchronize with an advertiser's schedule to receive data deterministically without a connection (like a beacon but more reliable).

### 4.3 LE Coded PHY (Long Range)
Uses **Forward Error Correction (FEC)** to increase range at the cost of data rate.
*   **S=2**: 500 kbps (approx 2x range).
*   **S=8**: 125 kbps (approx 4x range).

### 4.4 LE 2M PHY (High Speed)
Doubles the symbol rate to 2 Ms/s, increasing throughput and reducing power consumption (radio is on for less time).
