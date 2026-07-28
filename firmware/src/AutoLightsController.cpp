#include "AutoLightsController.h"

AutoLightsController::AutoLightsController(ModuleConfig config)
    : config_(config) {}

LightCommand AutoLightsController::update(const VehicleState& state) {
  const bool wasEngineRunning = previousEngineRunning_;
  previousEngineRunning_ = state.engineRunning;

  if (!autoSelected(state)) {
    enterMode(ControllerMode::Idle, state.nowMs);
    return {};
  }

  if (state.unlockedEvent && config_.welcomeEnabled &&
      (!config_.welcomeOnlyDark || state.dark)) {
    enterMode(ControllerMode::Welcome, state.nowMs);
  }

  if (wasEngineRunning && !state.engineRunning && config_.goodbyeEnabled &&
      (!config_.goodbyeOnlyDark || state.dark)) {
    enterMode(ControllerMode::Goodbye, state.nowMs);
  }

  if (state.engineRunning) {
    if (mode_ != ControllerMode::Drl) {
      enterMode(ControllerMode::Drl, state.nowMs);
    }

    if (state.nowMs - modeStartedAtMs_ < config_.drlStartDelayMs) {
      return {};
    }
    return commandFromSelection(config_.drlLights);
  }

  switch (mode_) {
    case ControllerMode::Welcome:
      if (modeExpired(state, config_.welcomeDurationMs)) {
        enterMode(state.ignitionOn ? ControllerMode::Acc : ControllerMode::Idle,
                  state.nowMs);
      } else {
        return commandFromSelection(config_.welcomeLights);
      }
      break;

    case ControllerMode::Goodbye:
      if (modeExpired(state, config_.goodbyeDurationMs)) {
        enterMode(state.ignitionOn ? ControllerMode::Acc : ControllerMode::Idle,
                  state.nowMs);
      } else {
        return commandFromSelection(config_.goodbyeLights);
      }
      break;

    default:
      break;
  }

  if (state.ignitionOn) {
    enterMode(ControllerMode::Acc, state.nowMs);
    if (!config_.ignitionLightsEnabled) {
      return {};
    }
    return commandFromSelection(config_.ignitionLights);
  }

  enterMode(ControllerMode::Idle, state.nowMs);
  return {};
}

bool AutoLightsController::autoSelected(const VehicleState& state) const {
  return state.switchPosition == SwitchPosition::Auto;
}

bool AutoLightsController::modeExpired(const VehicleState& state,
                                       uint32_t durationMs) const {
  return state.nowMs - modeStartedAtMs_ >= durationMs;
}

void AutoLightsController::enterMode(ControllerMode mode, uint32_t nowMs) {
  if (mode_ == mode) {
    return;
  }

  mode_ = mode;
  modeStartedAtMs_ = nowMs;
}

LightCommand AutoLightsController::commandFromSelection(
    LightSelection selection) const {
  LightCommand command;
  command.active = selection != LightSelection::Off;
  command.sidelights = hasLight(selection, LightSelection::Sidelights);
  command.lowBeam = hasLight(selection, LightSelection::LowBeam);
  command.fog = hasLight(selection, LightSelection::Fog);
  return command;
}
