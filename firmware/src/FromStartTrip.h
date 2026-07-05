#pragma once

#include <stdint.h>

#include "VehicleState.h"

struct FromStartTripSnapshot {
  bool active = false;
  uint32_t elapsedMs = 0;
  float distanceKm = 0.0f;
  float fuelLiters = 0.0f;
  float averageSpeedKph = 0.0f;
  float averageConsumptionLPer100Km = 0.0f;
};

class FromStartTrip {
 public:
  explicit FromStartTrip(uint32_t resetAfterMs);

  FromStartTripSnapshot update(const VehicleState& state);
  void reset(uint32_t nowMs);

 private:
  uint32_t resetAfterMs_;
  bool sessionActive_ = false;
  bool previousEngineRunning_ = false;
  uint32_t sessionStartedAtMs_ = 0;
  uint32_t lastEngineStopAtMs_ = 0;
  uint32_t lastUpdateMs_ = 0;
  bool hasLastUpdate_ = false;
  float distanceKm_ = 0.0f;
  float fuelLiters_ = 0.0f;

  void startSession(uint32_t nowMs);
  void integrate(const VehicleState& state);
  FromStartTripSnapshot snapshot(const VehicleState& state) const;
};
