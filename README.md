# bluetooth-battery-monitor

Bluetooth Battery Monitor (BTMonitor)

created by mLace 
dev website https://mlace.com
BTMonitor website https://mlace.com/btmonitor/

A small utility to monitor battery levels of Bluetooth devices using UPower.

Bluetooth Battery Monitor (BTMonitor) is a simple battery level and charging monitor for wireless bluetooth devices.

BTMonitor uses a device's specific MAC address to check the battery level so any wireless device you know the MAC address to can be displayed.

In order to find your connected devices' MAC address, use bluetoothctl via the command line to see connected devices via list:

Steps to Find Bluetooth MAC Address:

    1] Open Terminal: Press Ctrl+Alt+T.
    2] Start Bluetooth Controller: Type bluetoothctl and press Enter.
    3] List Known Devices: Type devices to list previously paired devices and their MAC addresses.
    4] Scan for New Devices: If the device isn't listed, turn on pairing mode on your device and type scan on in the terminal to find it.
    5] Identify MAC: The output will show the MAC address in the format XX:XX:XX:XX:XX:XX next to the device name.
    6] Exit: Type quit to exit bluetoothctl.

Installation:

  1] Download the .deb from https://mlace.com/btmonitor/bluetooth-battery-monitor_0.01a09_amd64.deb

  2] Continue the install via the App Center. (apt should install all dependencies)
  
