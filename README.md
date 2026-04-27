# Bluetooth Battery Monitor (BTMonitor)

**BTMonitor** is a simple battery level and charging monitor for wireless Bluetooth devices, created by **mLace**. It utilizes `UPower` and specific device MAC addresses to track battery status in real-time.

---

## 📸 Screenshot
| Main Screen | In Use | About |
| :---: | :---: | :---: |
| <img src="https://mlace.com/btmonitor/btmonitorabout.png" width="250" > | <img src="https://mlace.com/btmonitor/btmonitorinuse.png" width="250" /> | <img src="https://mlace.com/btmonitor/btmonitormainscreen.png" width="250"> | 

---

## 🚀 Features
- Monitor battery percentage and charging status.
- Support for any Bluetooth device with a known MAC address.
- Lightweight utility utilizing `UPower` integration.

---

## 🔍 How to Find Your Bluetooth MAC Address
To monitor a device, you need its unique MAC address. You can find this using `bluetoothctl` via the command line:

1.  **Open Terminal:** Press `Ctrl+Alt+T`.
2.  **Start Bluetooth Controller:** Type `bluetoothctl` and press **Enter**.
3.  **List Known Devices:** Type `devices` to list previously paired devices and their MAC addresses.
4.  **Scan for New Devices:** If your device isn't listed, put it in pairing mode and type `scan on`.
5.  **Identify MAC:** Find the address in the format `XX:XX:XX:XX:XX:XX` next to your device name.
6.  **Exit:** Type `quit` to exit.

---

## 📥 Installation

1.  **Download:** Grab the latest `.deb` package from the [official website](https://mlace.com/btmonitor/bluetooth-battery-monitor_0.01a09_amd64.deb).
2.  **Install:** Open the downloaded file with your system **App Center** or install via terminal:
    ```bash
    sudo apt install ./bluetooth-battery-monitor_0.01a09_amd64.deb
    ```
    *(Apt will automatically handle any necessary dependencies.)*

---

## 🔗 Project Links
- **Developer Website:** [mlace.com](https://mlace.com)
- **BTMonitor Home:** [mlace.com/btmonitor](https://mlace.com/btmonitor)

---
