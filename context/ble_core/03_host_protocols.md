# BLE Host Protocols

The Host layers provide high-level abstractions for applications to interact with BLE devices without managing radio timing.

## 1. L2CAP (Logical Link Control and Adaptation Protocol)
Acts as a protocol multiplexer. It takes data from upper layers (ATT, SM) and packages it for the Controller.
*   **Multiplexing**: Using Logical Channels (CIDs).
    *   **CID 0x0004**: Attribute Protocol (ATT).
    *   **CID 0x0006**: Security Manager (SM).
*   **segmentation and Reassembly (SAR)**: Splits large packets into smaller chunks that fit the Controller's maximum buffer size (MTU).

## 2. ATT (Attribute Protocol) & GATT (Generic Attribute Profile)
The heart of BLE data exchange.

### 2.1 ATT Model
Defines how data is stored and accessed.
*   **Server**: Holds the data (Attributes).
*   **Client**: Requests data (Read/Write) or receives updates (Notify/Indicate).
*   **Attribute Structure**:
    *   **Handle**: 16-bit address (e.g., 0x002A).
    *   **Type (UUID)**: What the data is (e.g., Heart Rate Measurement).
    *   **Value**: The actual data.
    *   **Permissions**: Read, Write, Encryption required, etc.

### 2.2 GATT Hierarchy
Organizes Attributes into a logical structure:
1.  **Profile**: A collection of Services (e.g., Heart Rate Profile).
2.  **Service**: A collection of Characteristics (e.g., Heart Rate Service).
3.  **Characteristic**: A data value (e.g., Heart Rate Measurement).
    *   **Declaration**: Metadata.
    *   **Value**: Actual data.
    *   **Descriptor**: Configuration (e.g., CCCD for enabling notifications).

### 2.3 Operations
*   **Read / Write**: Client initiated.
*   **Notify**: Server initiated, no acknowledgement (Fast).
*   **Indicate**: Server initiated, requires acknowledgement (Reliable).

## 3. GAP (Generic Access Profile)
Defines how devices discover and connect to each other.
*   **Modes**:
    *   **Discoverable**: Broadcasts advertising packets.
    *   **Connectable**: Accepts connection requests.
*   **Security Modes**:
    *   **Mode 1**: Encryption required (Level 1: No Security, Level 2: Unauthenticated, Level 3: Authenticated, Level 4: LESC).
    *   **Mode 2**: Data Signing required.

## 4. SM (Security Manager)
Handles Pairing (Key Exchange) and Bonding (Storing Keys).

### 4.1 Pairing Methods
*   **Just Works**: No user interaction. Unauthenticated against eavesdropping (unless using LESC).
*   **Passkey Entry**: User enters a 6-digit code. Authenticated (MITM protection).
*   **Numeric Comparison** (LESC only): User confirms codes match. Authenticated.
*   **OOB (Out of Band)**: Uses NFC or other channel. Highly secure.

### 4.2 Legacy vs. LE Secure Connections (LESC)
*   **Legacy**: Uses AES-128 but key exchange is vulnerable to sniffing.
*   **LESC (BLE 4.2+)**: Uses Elliptic Curve Diffie-Hellman (ECDH) for secure key exchange, preventing passive eavesdropping.
