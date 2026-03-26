# Programmable Mechanical Macropad

The **Programmable Mechanical Macropad** is a high-performance productivity tool featuring custom bare-metal firmware developed for the **ESP32-S3**. This project replaces standard keyboard frameworks with a specialized C++ architecture designed for independent switch matrix scanning, hardware debouncing, and on-the-fly macro execution.

## System Architecture
The firmware is designed for low-latency input processing, utilizing a modular structure to separate hardware interaction from core logic.
### 1. Hardware Input & Matrix Scanning (`Firmware/Inputs.h`)
* **Matrix Scanning:** Implements a high-frequency polling routine to scan the mechanical switch matrix independently of standard libraries.
* **Hardware Debouncing:** Custom algorithms filter electrical noise from mechanical switches to ensure precise, single-strike key registration.
### 2. Action Engine & Macro Execution (`Firmware/Actions.h`)
* **Macro Logic:** Manages the execution of complex key sequences and software shortcuts mapped to specific physical keys.
* **On-the-Fly Configuration:** Allows for the execution of stored macros without requiring a full firmware re-flash.
### 3. State & Visual Feedback (`Firmware/Display.h` & `Storage.h`)
* **Persistent Storage:** Utilizes the ESP32-S3's internal flash to save user-defined key mappings and macro profiles.
* **Visual Interface:** Coordinates with an external display module to provide real-time status updates on active profiles or layers.

## Technical Stack
| Category | Specifications |
| :--- | :--- |
| **Microcontroller** | ESP32-S3 (Dual-Core Xtensa LX7)  |
| **Languages** | Bare-metal C++  |
| **Input Tech** | Mechanical Switch Matrix with Custom Scanning  |
| **Features** | Hardware Debouncing and Macro Execution Engine  |
| **Storage** | Non-Volatile Flash for Profile Persistence |

## Repository Structure
The codebase is organized into a modular firmware directory to separate hardware initialization from core logic.
```text
Mechanical_Macropad/
├── Firmware/                 # ESP32-S3 Bare-Metal Logic
│   ├── SmartDeck.ino         # Main system lifecycle and entry point
│   ├── Inputs.h              # Switch matrix scanning and debouncing logic
│   ├── Actions.h             # Macro engine and shortcut execution
│   ├── Storage.h             # Flash memory management for key profiles
│   ├── Display.h             # Visual feedback and UI orchestration
│   └── Config.h              # Pin mapping and system constants
├── LICENSE                   # MIT Open Source License
└── README.md                 # Project Documentation
```

## Deployment & Setup
### 1. Development Environment
* **PlatformIO / Arduino IDE:** Ensure the **ESP32-S3** board support package is installed.
* **HID Support:** While the firmware uses a custom matrix architecture, ensure standard Human Interface Device (HID) support is enabled in your build flags.
### 2. Firmware Initialization
1.  **Pin Configuration:** Open `Firmware/Config.h` and verify the GPIO assignments for your specific switch matrix rows and columns.
2.  **Compilation:** Connect the ESP32-S3 via USB-C and upload the `SmartDeck.ino` sketch.
3.  **Serial Monitor:** Use a baud rate of **115200** to verify the hardware debouncing and scanning initialization.
### 3. Hardware Assembly
* **Switch Matrix:** Wire your mechanical switches in a grid pattern as defined in your hardware configuration[cite: 35].
* **Anti-Ghosting:** Ensure signal diodes (e.g., 1N4148) are used at each switch node to prevent "ghosting" during multi-key macro execution.
* **Display (Optional):** If using the display module, connect the I2C/SPI pins as specified in `Display.h`.

## Professional Profile
* **Developer:** Ritul Raj Bhakat (Firmware Developer)
* **Innovation:** Engineered custom bare-metal firmware for an ESP32-S3 to manage macro execution without third-party frameworks.
* **Contact:** [ritulraj384@gmail.com](mailto:ritulraj384@gmail.com) | [LinkedIn](https://www.linkedin.com/in/ritul-raj-bhakat-521202277/) | [Github](https://github.com/Chikkkuuu) | [Portfolio](https://ritulrajbhakatportfolio.vercel.app/)
