#include <cassert>

#include "AutoLightsController.h"
#include "FromStartTrip.h"

namespace {

VehicleState baseState(uint32_t nowMs) {
  VehicleState state;
  state.nowMs = nowMs;
  state.switchPosition = SwitchPosition::Auto;
  state.dark = true;
  return state;
}

void testNoOutputWhenNotAuto() {
  ModuleConfig config;
  AutoLightsController controller(config);

  VehicleState state = baseState(100);
  state.switchPosition = SwitchPosition::LowBeam;
  state.ignitionOn = true;
  state.engineRunning = true;

  const LightCommand command = controller.update(state);
  assert(!command.active);
  assert(controller.mode() == ControllerMode::Idle);
}

void testNoLowBeamOnIgnitionOnlyByDefault() {
  ModuleConfig config;
  AutoLightsController controller(config);

  VehicleState state = baseState(100);
  state.ignitionOn = true;
  state.engineRunning = false;

  const LightCommand command = controller.update(state);
  assert(!command.active);
  assert(controller.mode() == ControllerMode::Acc);
}

void testConfigurableIgnitionLights() {
  ModuleConfig config;
  config.ignitionLights = LightSelection::Sidelights;
  AutoLightsController controller(config);

  VehicleState state = baseState(100);
  state.ignitionOn = true;
  state.engineRunning = false;

  const LightCommand command = controller.update(state);
  assert(command.active);
  assert(command.sidelights);
  assert(!command.lowBeam);
  assert(controller.mode() == ControllerMode::Acc);
}

void testDrlAfterEngineRunningDelay() {
  ModuleConfig config;
  config.drlStartDelayMs = 500;
  AutoLightsController controller(config);

  VehicleState state = baseState(1000);
  state.ignitionOn = true;
  state.engineRunning = true;

  LightCommand command = controller.update(state);
  assert(!command.active);
  assert(controller.mode() == ControllerMode::Drl);

  state.nowMs = 1600;
  command = controller.update(state);
  assert(command.active);
  assert(command.lowBeam);
  assert(command.sidelights);
  assert(!command.fog);
}

void testWelcomeOnlyDark() {
  ModuleConfig config;
  config.welcomeLights = LightSelection::Sidelights;
  AutoLightsController controller(config);

  VehicleState state = baseState(1000);
  state.dark = false;
  state.unlockedEvent = true;

  LightCommand command = controller.update(state);
  assert(!command.active);

  state = baseState(2000);
  state.dark = true;
  state.unlockedEvent = true;
  command = controller.update(state);
  assert(command.active);
  assert(command.sidelights);
  assert(!command.lowBeam);
}

void testFromStartTripKeepsSessionUnderResetTimeout() {
  FromStartTrip trip(5UL * 60UL * 60UL * 1000UL);

  VehicleState state = baseState(0);
  state.engineRunning = true;
  state.speedKph = 60.0f;
  state.fuelRateLitersPerHour = 6.0f;
  trip.update(state);

  state.nowMs = 60UL * 60UL * 1000UL;
  trip.update(state);

  state.engineRunning = false;
  state.nowMs += 1000;
  trip.update(state);

  state.engineRunning = true;
  state.nowMs += 2UL * 60UL * 60UL * 1000UL;
  const FromStartTripSnapshot snapshot = trip.update(state);

  assert(snapshot.active);
  assert(snapshot.distanceKm > 59.0f);
}

void testFromStartTripResetsAfterTimeout() {
  FromStartTrip trip(5UL * 60UL * 60UL * 1000UL);

  VehicleState state = baseState(0);
  state.engineRunning = true;
  state.speedKph = 60.0f;
  state.fuelRateLitersPerHour = 6.0f;
  trip.update(state);

  state.nowMs = 60UL * 60UL * 1000UL;
  trip.update(state);

  state.engineRunning = false;
  state.nowMs += 1000;
  trip.update(state);

  state.engineRunning = true;
  state.nowMs += 6UL * 60UL * 60UL * 1000UL;
  const FromStartTripSnapshot snapshot = trip.update(state);

  assert(snapshot.active);
  assert(snapshot.distanceKm < 1.0f);
  assert(snapshot.fuelLiters < 0.1f);
}

}  // namespace

int main() {
  testNoOutputWhenNotAuto();
  testNoLowBeamOnIgnitionOnlyByDefault();
  testConfigurableIgnitionLights();
  testDrlAfterEngineRunningDelay();
  testWelcomeOnlyDark();
  testFromStartTripKeepsSessionUnderResetTimeout();
  testFromStartTripResetsAfterTimeout();
  return 0;
}
