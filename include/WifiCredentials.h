#pragma once

#if __has_include("secrets.h")
#include "secrets.h"
#define HAS_SECRETS
#define WIFI_CREDENTIALS SECRET_SSID, SECRET_PASS
#else
#define WIFI_CREDENTIALS nullptr, nullptr
#endif
