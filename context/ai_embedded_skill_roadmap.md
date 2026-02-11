# AI Embedded Skill Roadmap: From Text to Structured Logic

**Objective**: Shift from "Probabilistic Text Generation" to "Structured Data-Based Logical Reasoning".
**Core Principle**: Embedded development requires strict adherence to hardware physical constraints. Textual approximation is unacceptable.

## 1. Knowledge Input Transformation: PDF $\rightarrow$ Digital Twin

Standard LLMs fail at complex PDF tables. We must feed the AI strict, structured data.

*   **Register Level (Truth Source)**: **CMSIS-SVD** (System View Description).
    *   *Action*: Do not rely on text descriptions of registers. Input `.svd` (XML) files directly.
    *   *Content*: Physical addresses, Bit fields, Reset values.
*   **Pin Level (Connectivity)**: **Netlist as Graph**.
    *   *Action*: Convert text netlists to **Text-Attributed Graphs (TAG)**.
    *   *Content*: Nodes (Pins), Edges (Connections), Attributes (Drive strength, Pull-up/down).
*   **Visual Level (Geometry)**: **Geometric Reasoning**.
    *   *Action*: Use Vision-LLMs (e.g., LLM4-IC8K) to calculate pin coordinates and recognize package physical constraints.

## 2. Hierarchical Agent Architecture (SpecMap)

Decompose the monolithic "Coding" task into specialized expert agents.

1.  **Parsing Agent**:
    *   *Input*: Datasheet / Pin Mux Table.
    *   *Output*: Standard JSON.
    *   *Behavior*: Aggressively questions ambiguity (e.g., "P0.14 conflict: Used for Battery AND GPIO?").
2.  **Mapping Agent**:
    *   *Input*: Netlist Graph + SVD.
    *   *Output*: **Connection Matrix**.
    *   *Behavior*: Maps logical signals to physical pins/peripherals without generating code yet.
3.  **Constraint Agent**:
    *   *Input*: Connection Matrix + Electrical Rules.
    *   *Output*: Validated Matrix.
    *   *Behavior*: Enforces strict rules (e.g., "I2C requires High Drive pins", "No SPI on QSPI lines").

## 3. Grammar-Constraint Decoding (GCD)

Prevent "Hallucinations" by making invalid output mathematically impossible.

*   **Logit Masking**:
    *   If Target = nRF52840 (Max Pin P1.15), **Mask** any token representing P1.16+.
*   **Context-Aware Parsing**:
    *   Enforce Context-Free Grammar (CFG).
    *   Example: A register write `NRF_TIMER0->CC[0]` is only valid if `TIMER0` exists in the loaded SVD.

## 4. The "Generate-Verify-Feedback" Loop

Imitate a senior engineer's "Compile-Fix" cycle.

1.  **Formal Verification**:
    *   *Action*: Generate DeviceTree (`.dts`).
    *   *Check*: Run `dtc` (DeviceTree Compiler).
    *   *Feedback*: If `dtc` errors ("Node label undefined"), feed raw error back to AI for self-correction.
2.  **Simulation**:
    *   *Action*: Export netlist to LTSpice/Spectre.
    *   *Check*: Verify timing/logic levels.

## 5. Hardware-Aware Knowledge Base (KB)

Structure the KB as a **3-Dimensional Map**, not flat text.

*   **Dimension 1 (Hardware)**: Logical Pin $\rightarrow$ Physical Pin $\rightarrow$ Peripheral Channel.
*   **Dimension 2 (Norms)**: "Gotchas" (e.g., NFC pins default to antenna, not GPIO).
*   **Dimension 3 (Evolution)**: Version control for Chips (nRF52 RTC $\neq$ nRF54 GRTC).

---

## Application to This Project
To act as this "Structured AI", we will:
1.  **Prioritize `.dts` / `.overlay` files** as the source of truth for pinouts.
2.  **Simulate Agents**: Explicitly step through "Parsing", "Mapping", and "Verifying" steps in our Chain of Thought.
3.  **Treat SVD as Bible**: When available, refer to register definitions over descriptive text.
