# Programmable Mechanical Macropad (SmartDeck)
### ESP32-S3 Bare-Metal Firmware | Custom HID Engine
**Focus:** Low-latency switch matrix scanning, non-volatile macro storage, and hardware-level debouncing.

---

## Engineering Philosophy
Standard keyboard frameworks often carry significant overhead that can introduce input jitter. The **SmartDeck** firmware was engineered from the ground up to prioritize **input determinism**. By bypassing heavy abstractions and utilizing direct register access, I achieved a high-frequency polling rate that ensures macro execution begins within microseconds of physical switch actuation.

---

## Technical Challenges & Engineering Solutions

### 1. Deterministic Matrix Scanning vs. Ghosting
*   **The Problem:** In a standard 3x3 grid, pressing multiple keys simultaneously can create "ghost" keypresses due to sneak paths.
*   **The Solution:** Implemented a diode-protected matrix logic. The firmware uses an optimized **active-low polling routine** in `Inputs.h` that scans rows and reads columns in under **1ms**, ensuring N-Key Rollover (NKRO) capability at the firmware level.

### 2. Temporal Hardware Debouncing
*   **The Problem:** Mechanical switches suffer from contact bounce, which can trigger macros multiple times per single strike.
*   **The Solution:** Rather than relying on external RC circuits, I developed a **software-based temporal debounce algorithm**. It utilizes a per-key state counter that only registers a "Press" or "Release" once the signal remains stable across 5 consecutive scans, eliminating chatter without compromising response speed.

### 3. Flash Wear Leveling for Persistence
*   **The Problem:** User-defined macros are stored in the ESP32-S3’s internal flash. Frequent updates to key-mappings can prematurely wear out specific memory sectors.
*   **The Solution:** Architected `Storage.h` to interface with the **Non-Volatile Storage (NVS)** partition. This utilizes the ESP32's internal wear-leveling logic to distribute write cycles across the flash chip, significantly extending the hardware's operational lifespan.

---

## System Architecture

### 1. The Action Engine (`Actions.h`)
*   **Macro Execution:** Manages complex multi-key sequences (e.g., `Ctrl+Shift+T`) through an asynchronous execution queue.
*   **Layer Logic:** Supports dynamic "Function Layers," allowing the 9-key pad to map to 27+ unique software commands via toggle keys.

### 2. Dual-Core Orchestration
To ensure input latency remains unaffected by visual updates:
*   **Core 0:** Dedicated to high-speed Matrix Scanning and HID reporting.
*   **Core 1:** Manages the I2C OLED Display and NVS Flash writes.

---

## Performance & Specs
*   **MCU:** ESP32-S3 (Dual-Core 240MHz Xtensa LX7).
*   **Scan Rate:** >1kHz (Sub-1ms latency).
*   **Debounce Window:** 5ms (User-configurable in `Config.h`).
*   **Interface:** USB-C HID (Native Support).

---

## Repository Structure
```text
Mechanical_Macropad/
├── Firmware/                 # Bare-Metal C++ Architecture
│   ├── SmartDeck.ino         # Main Lifecycle & Dual-Core Scheduler
│   ├── Inputs.h              # Matrix Scan & Debounce Logic
│   ├── Actions.h             # Macro Processing Engine
│   ├── Storage.h             # NVS Partition Management
│   └── Config.h              # Pin Mapping & System Constants
```

---

## 💡 Lessons Learned & Future Iterations
This project served as a deep dive into optimizing the I/O bottleneck of the ESP32-S3:

*   **The Pivot:** Initially, I relied on the standard Arduino `digitalRead()` function. Benchmarking revealed that the overhead of this high-level abstraction was too significant for a high-performance HID. I pivoted to **Direct Register Access (GPIO bit-masking)**, which slashed scan times by **~70%** and moved the project into a professional-grade performance tier.
*   **Future Refinement:** I am currently exploring **TinyUSB** integration as a standalone component. This will allow for deeper control over custom HID descriptors, enabling advanced features like multi-point consumer control (media keys) and raw HID data streams.

---

## 📬 Contact & Proof of Work
**Ritul Raj Bhakat**  
*Firmware Developer | Embedded Systems Architect*

*   **Deep Dive:** [View My Full Portfolio](https://ritulrajbhakatportfolio.vercel.app/)
*   **Professional:** [LinkedIn](https://linkedin.com/in/ritul-raj-bhakat)
*   **Direct:** [Email Me](mailto:ritulraj384@gmail.com)

---
© 2026 Ritul Raj Bhakat. Built as a high-performance productivity tool.
