# gitlab-pipeline-semaphor

Firmware for a GitLab pipeline status indicator, running on an ESP8266 (NodeMCU v2). A relay-driven traffic light and a TFT display show the status of the latest pipeline, and the firmware updates itself from GitHub Releases.

## Hardware

- ESP8266 NodeMCU v2
- 3x relay (red, yellow, green)
- TFT display (ST7789, 240x280) over SPI
- Backlight controlled by a GPIO pin

Wiring (can be changed in [include/config.h](include/config.h) and [platformio.ini](platformio.ini)):

| Signal | GPIO |
|---|---|
| Red relay | 15 |
| Yellow relay | 0 |
| Green relay | 16 |
| Display backlight | 2 |
| Display MOSI | 13 |
| Display SCLK | 14 |
| Display CS | 12 |
| Display DC | 4 |
| Display RST | 5 |

## Behavior

Every 10 s the device fetches the latest pipeline from `API_URL` and shows its status:

| Pipeline status | Light | Display |
|---|---|---|
| running, pending, scheduled, manual | yellow | "WAITING" + author |
| success | green | "SUCCESS" + author |
| failed | red | "FAILED" + author |
| created, waiting_for_resource, preparing, canceled, skipped | unchanged | unchanged |

On a request or parsing error all three lights turn off.

## Configuration

Everything is in [include/config.h](include/config.h): the GitLab API URL, poll interval, request timeout and relay/backlight pins. Display pins and SPI settings are build flags in [platformio.ini](platformio.ini).

## Font

The display needs `Charis_SILR.vlw` in SPIFFS. Upload it once over USB:

```
pio run -t uploadfs
```

OTA updates only replace the firmware, not the filesystem, so this step is not repeated by GitHub Actions.

## WiFi and GitLab token

Create `include/secrets.h` (it is in `.gitignore`):

```cpp
#define SECRET_SSID "wifi-name"
#define SECRET_PASS "wifi-password"

#define SECRET_GITLAB_TOKEN "gitlab-personal-access-token"
```

On the first upload over USB the WiFi credentials and the GitLab token are saved to EEPROM. Firmware built by GitHub Actions has no `secrets.h` and uses the saved values. To change them, edit `secrets.h` and upload the firmware over USB again.

## Build and upload

```
pio run -t upload
pio device monitor
```

On startup the yellow light turns on and the display shows "WELCOME" while the device connects to WiFi and checks for updates. Normal polling then takes over. The firmware also prints its version and the result of the update check to the serial monitor.

## Source layout

- `src/main.cpp` – `setup()` and `loop()` only
- `src/Pipeline.cpp` – fetches and parses the pipeline status, drives the light and display
- `src/Semaphore.cpp` – relay control
- `src/Display.cpp` – TFT output and the SPIFFS font
- `src/TokenStore.cpp` – persists the GitLab token in EEPROM

## Firmware update (OTA)

Uses the [esp-ota-updater](https://github.com/davidvancl/esp-ota-updater) library. On startup the device checks the latest release and updates itself if a newer version exists.

Releasing a new version:
1. Increase `custom_version` in `platformio.ini`.
2. Commit and push to `main`.
3. GitHub Actions publish a release. The device updates after its next restart.

Do not push a locally changed version in `platformio.ini` unless you want to publish a release.
