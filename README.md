
# Heltec WL-32 LoRa Tracker

This repository contains the code for the Heltec WL-32 LoRa Tracker, a prototype platform used in the outdoor communication game [RadioGrid.Run](https://radiogrid.run). This code is designed to run on the ESP32-based, GPS-enabled pocket device carried by teams participating in the game.

## Overview

The Heltec WL-32 LoRa Tracker is used as a test platform in order to add location tracking of the teams using LoRaWAN (TTN) by the base. This repository is a work in progress.

## Features

- **LoRa Communication:** Utilize LoRa technology for long-range, low-power communication.
- **GPS Tracking:** Accurate location tracking with built-in GPS.
- **Real-time Data Sharing:** Share location and other game-related data in real time.
- **ESP32-Based:** Powered by the ESP32 microcontroller.

## Getting Started

### Prerequisites

- Heltec WiFi LoRa 32 (V2) device
- PlatformIO installed on your computer
- Required libraries:
  - `Heltec ESP32`
  - `TinyGPS++`
  - `LoRa`
  

## Contributing

We welcome contributions to improve the Heltec WL-32 LoRa Tracker code. But be aware, that this project is at a very early stage. The goal of this code is to ensure that the logging infrastructure (not described here) make sens.
But, if you still want to follow allong, please follow these steps:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature/your-feature-name`).
3. Make your changes and commit them (`git commit -m 'Add your feature'`).
4. Push to the branch (`git push origin feature/your-feature-name`).
5. Create a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For any questions or support, please open an issue on this repository or contact the maintainers through the [RadioGrid.Run](https://radiogrid.run) website. A basic documentation on the game rules is to be found on this repository: [https://github.com/radio-grid-run/radio-grid-run-DOC](radio-grid-run)

---

Thank you for your interest for the Heltec WL-32 LoRa Tracker in your RadioGrid.Run adventures!
