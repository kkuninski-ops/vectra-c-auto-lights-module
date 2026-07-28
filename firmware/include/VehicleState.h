#pragma once

#include <stdint.h>

enum class SwitchPosition : uint8_t {
  Unknown,
  Off,
  Auto,
  Sidelights,
  LowBeam,
  Fog,
};

struct VehicleState {
  uint32_t nowMs = 0;

  SwitchPosition switchPosition = SwitchPosition::Unknown;

  bool ignitionOn = false;
  bool engineRunning = false;
  bool unlockedEvent = false;
  bool lockedEvent = false;
  bool dark = false;
  bool factoryDimmingActive = false;

  uint16_t ambientRaw = 0;
  float batteryVoltage = 0.0f;
  float speedKph = 0.0f;
  float fuelRateLitersPerHour = 0.0f;
};

struct LightCommand {
  bool active = false;
  bool sidelights = false;
  bool lowBeam = false;
  bool fog = false;
};
