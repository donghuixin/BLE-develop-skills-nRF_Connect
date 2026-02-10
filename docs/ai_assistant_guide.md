# How to Use This Library with AI Assistants

This guide explains how to leverage this "Skill" library to make Cursor, GitHub Copilot, or ChatGPT an expert in nRF Connect SDK.

## 1. The "System Prompt" Injection
The core of this strategy is the **Master System Prompt**.

### For Cursor / ChatGPT (Custom Instructions)
1.  Open `prompts/master_system_prompt.md`.
2.  Copy the entire content.
3.  **Cursor**: Go to `Settings` -> `General` -> `Rules for AI` and paste it there.
4.  **ChatGPT**: Go to `Settings` -> `Custom Instructions` and paste it in the "How would you like ChatGPT to respond?" section.

**Why this works**: It forces the AI to check for `device_is_ready`, use DeviceTree macros, and always output the 3-file structure (`main.c`, `prj.conf`, `app.overlay`).

## 2. Context Injection (RAG - Retrieval Augmented Generation)
When you have a specific task, "feed" the relevant context file to the AI.

### Scenario: "I want to use the ADC"
1.  Open your chat.
2.  Type: "I need to read an analog sensor on P0.05 using the ADC."
3.  **Crucial Step**: Attach or Paste content from `context/zephyr_basics.md` (or any specific ADC context if you add one).
4.  The AI will now use the correct `ADC_DT_SPEC_GET` macros instead of making up invalid code.

### Scenario: "Datasheet Lookup"
The system prompt is now trained to look at `context/nordic_docs_index.md`.
-   If you ask about **nRF9160**, it knows to check the specific Product Spec.
-   If it needs general info, it knows to go to [https://docs.nordicsemi.com/bundle?labelkey=datasheet](https://docs.nordicsemi.com/bundle?labelkey=datasheet).

## 3. Automation with "Slash Commands" (Concept)
In Cursor, you can create a `.cursorrules` file in your project root that references these prompts, effectively creating a local "Skill".

## 4. JLink / Terminal Workflow
We've included `scripts/flash_and_monitor.sh`.
-   **VS Code**: Press `Cmd+Shift+P` -> `Tasks: Run Task` -> `Flash & Monitor`.
-   **Terminal**: Run `./scripts/flash_and_monitor.sh`.

This automatically flashes the `build` directory and attempts to open a serial monitor, saving you from typing `nrfjprog --rtt` manually every time.
