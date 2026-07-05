#pragma once

#include "Config.h"
#include "VehicleState.h"

enum class ControllerMode {
  Idle,
  Welcome,
  Acc,
  Drl,
  Goodbye,
};

class AutoLightsController {
 public:
  explicit AutoLightsController(ModuleConfig config);

  LightCommand update(const VehicleState& state);
  ControllerMode mode() const { return mode_; }

 private:
  ModuleConfig config_;
  ControllerMode mode_ = ControllerMode::Idle;
  uint32_t modeStartedAtMs_ = 0;
  bool previousEngineRunning_ = false;

  bool autoSelected(const VehicleState& state) const;
  bool modeExpired(const VehicleState& state, uint32_t durationMs) const;
  void enterMode(ControllerMode mode, uint32_t nowMs);
  LightCommand commandFromSelection(LightSelection selection) const;
};
