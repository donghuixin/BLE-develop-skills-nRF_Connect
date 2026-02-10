# ⚡️ embedded_Skill_nRF_Connect

<div align="center">

![Nordic](https://img.shields.io/badge/Nordic-nRF54%20%2F%20nRF53%20%2F%20nRF52-blue)
![Zephyr](https://img.shields.io/badge/RTOS-Zephyr-purple)
![License](https://img.shields.io/badge/License-MIT-green)
![Status](https://img.shields.io/badge/Status-Active-orange)

**The "Missing Manual" for nRF Connect SDK (NCS) Development.**  
*Bridging the gap between "Hello World" and Enterprise-Grade Firmware.*

[Getting Started](#-getting-started) •
[Philosophy](#-philosophy) •
[Architecture](#-architecture) •
[Star History](#-star-history)

</div>

---

## 🚀 Why this exists?

The transition from **nRF5 SDK** (Bare Metal) to **nRF Connect SDK** (Zephyr RTOS) is a massive paradigm shift. It requires unlearning "Register Poking" and mastering "DeviceTree & Kconfig".

This repository is an **AI-Enhanced Knowledge Base** and **Skill Library** designed to:
1.  **Enforce Best Practices**: "Hardware in Overlay, Software in Config".
2.  **Prevent Bricking**: Mandatory safety checks for UICR and Multi-core.
3.  **Scale Up**: From Blinky to Multi-image, Multi-core, IoT-Cloud applications.

## 🧠 The Knowledge Brain (`context/`)

Don't just copy-paste. Understand the architecture.

| Context File | Description |
| :--- | :--- |
| 🗺️ **[Knowledge Map](context/ncs_knowledge_map.md)** | **START HERE**. The Index of all knowledge. |
| 🛡️ **[Engineering Standards](context/ncs_engineering_standards.md)** | The "Constitution". Rules for Threading, Decoupling, and West. |
| 🧱 **[Project Structure](context/ncs_project_structure.md)** | `boards/`, `child_image/`, `src/modules/`. |
| 🔌 **[Peripherals](context/zephyr_peripherals.md)** | Gold-standard patterns for GPIO, I2C, SPI. |
| 🏛️ **[Real World Arch](context/real_world_architectures.md)** | Lessons from *OpenEarable*, *Seeed*, and *MakerDiary*. |

## 🧘 Philosophy

We believe in a **"Hardware-First, Safety-critical"** approach to firmware:

1.  **Truth is in the Overlay**: If a physical wire exists, it MUST be defined in `app.overlay`. We do not "hardcode" pins in C files.
2.  **OS Native**: We do not fight the RTOS. We use Zephyr's Drivers, Workqueues, and Logging subsystems instead of reinventing wheels.
3.  **Anti-Bricking**: We respect the hardware limits. We never touch UICR at runtime, and we validate power configurations before deployment.

## 🏗 Architecture

The system follows a strict layered architecture to ensure portability and scalability:

```mermaid
graph TD
    App[Application Logic] --> Managers[Managers / Services]
    Managers --> Drivers[Zephyr Device Drivers]
    Drivers --> HAL[Nordic nrfx HAL]
    HAL --> Hardware[nRF52/53/54 SoC]
    
    subgraph Configuration
    Kconfig[prj.conf (Features)]
    DTS[app.overlay (Hardware)]
    end
    
    Managers -.-> Kconfig
    Drivers -.-> DTS
```

-   **Application Layer**: Business logic (e.g., "Send Heart Rate"). Setup in `src/main.c` (Managers).
-   **Service Layer**: Reusable modules (e.g., "BLE Service", "Sensor Manager").
-   **Zephyr Kernel**: The abstraction layer (Scheduler, API).
-   **Hardware**: Defined strictly in **DeviceTree**.

## 📂 Project Structure

```text
├── .vscode/               # Recommended VS Code Settings
├── CLAUDE.md              # 🤖 AI Assistant "Driver's License"
├── context/               # The Knowledge Base (See above)
├── docs/                  # Usage Guides
├── examples/              # Verified Reference Code
│   ├── basic_blinky/      # The "Hello World" of NCS
│   └── i2c_sensor_read/   # The "Hardware First" workflow demo
├── prompts/               # System Prompts for customizing your AI
└── scripts/               # Automation tools (Flash/Monitor)
```

## 🛠 Getting Started

### 1. Prerequisites
-   **nRF Connect SDK** (v2.7.x or newer)
-   **VS Code** + nRF Connect Extension Pack
-   **West** tool installed.

### 2. Helper Scripts
Use our wrapper script to flash and monitor in one go:
```bash
./scripts/flash_and_monitor.sh <board_target>
```

## 🤖 AI Integration
This repo is "AI-Ready". 
1.  Copy `prompts/master_system_prompt.md` into your AI's custom instructions.
2.  The AI will now respect **Safety Rules** and **Zephyr Standards** automatically.
3.  Reference `CLAUDE.md` for quick context injection.

## 🌟 Star History

 <a href="https://star-history.com/#donghuixin/BLE-develop-skills-nRF_Connect&Date">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=donghuixin/BLE-develop-skills-nRF_Connect&type=Date&theme=dark" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=donghuixin/BLE-develop-skills-nRF_Connect&type=Date" />
   <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=donghuixin/BLE-develop-skills-nRF_Connect&type=Date" />
 </picture>
</a>
