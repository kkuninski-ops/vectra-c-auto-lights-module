#pragma once

#include <stdint.h>

enum class LightSelection : uint8_t {
  Off = 0,
  Sidelights = 1 << 0,
  LowBeam = 1 << 1,
  Fog = 1 << 2,
};

enum class BrightnessPolicy : uint8_t {
  FactoryManualLowBeam,
  ForceDayBrightness,
  ForceNightDimming,
  AutoByAmbientSensor,
};

inline LightSelection operator|(LightSelection lhs, LightSelection rhs) {
  return static_cast<LightSelection>(
      static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline bool hasLight(LightSelection value, LightSelection flag) {
  return (static_cast<uint8_t>(value) & static_cast<uint8_t>(flag)) != 0;
}

struct ModuleConfig {
  bool welcomeEnabled = true;
  bool welcomeOnlyDark = true;
  LightSelection welcomeLights = LightSelection::Sidelights;
  uint32_t welcomeDurationMs = 20000;

  LightSelection accLights = LightSelection::Off;

  uint32_t drlStartDelayMs = 500;
  LightSelection drlLights =
      LightSelection::LowBeam | LightSelection::Sidelights;

  bool goodbyeEnabled = true;
  bool goodbyeOnlyDark = true;
  LightSelection goodbyeLights = LightSelection::Sidelights;
  uint32_t goodbyeDurationMs = 20000;

  uint16_t darkThreshold = 1400;
  uint16_t lightThreshold = 1900;
  BrightnessPolicy brightnessPolicy = BrightnessPolicy::FactoryManualLowBeam;

  uint32_t fromStartResetAfterMs = 5UL * 60UL * 60UL * 1000UL;
};
