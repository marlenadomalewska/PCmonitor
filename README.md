# PC Monitor
LVGL 9 ESP32 project using WT32-SC01 screen.
### Parameters provider
[Hardware Monitor Tray by MayysterUFTD](https://github.com/MayysterUFTD/PC-parameters-serial-provider)
### secret/secrets.h file:
Used for WiFi connectivity. Replace with your credentials.
```
#ifndef SECRETS_H
#define SECRETS_H
const char *ssid = "your_ssid";
const char *password = "your_password";
#endif
```
