#include <Arduino.h>

#include "AutoLightsController.h"

namespace {

ModuleConfig config;
AutoLightsController controller(config);

VehicleState readVehicleState() {
  VehicleState state;
  state.nowMs = millis();

  // TODO: Replace these placeholders after measuring the light switch and
  // choosing the engine-running source.
  state.switchPosition = SwitchPosition::Unknown;
  state.ignitionOn = false;
  state.engineRunning = false;
  state.dark = false;
  state.ambientRaw = 0;
  state.batteryVoltage = 0.0f;

  return state;
}

void applyLightCommand(const LightCommand& command) {
  // TODO: Drive a fail-safe output stage that emulates factory switch states.
  // Do not wire this directly to lamps or xenon ballasts.
  (void)command;
}

void updateBleConfigurationService() {
  // TODO: Expose ModuleConfig over BLE GATT.
  // The phone app should write settings, not directly remote-control lamps.
}

}  // namespace

void setup() {
  Serial.begin(115200);
  Serial.println("Vectra C auto lights module boot");
}

void loop() {
  const VehicleState state = readVehicleState();
  const LightCommand command = controller.update(state);
  applyLightCommand(command);
  updateBleConfigurationService();

  delay(20);
}
