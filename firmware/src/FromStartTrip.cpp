#include "FromStartTrip.h"

FromStartTrip::FromStartTrip(uint32_t resetAfterMs)
    : resetAfterMs_(resetAfterMs) {}

FromStartTripSnapshot FromStartTrip::update(const VehicleState& state) {
  if (state.engineRunning && !previousEngineRunning_) {
    const bool expired =
        !sessionActive_ ||
        (lastEngineStopAtMs_ != 0 &&
         state.nowMs - lastEngineStopAtMs_ >= resetAfterMs_);

    if (expired) {
      reset(state.nowMs);
    }
    startSession(state.nowMs);
  }

  if (sessionActive_ && state.engineRunning) {
    integrate(state);
  }

  if (!state.engineRunning && previousEngineRunning_) {
    lastEngineStopAtMs_ = state.nowMs;
  }

  previousEngineRunning_ = state.engineRunning;
  lastUpdateMs_ = state.nowMs;
  return snapshot(state);
}

void FromStartTrip::reset(uint32_t nowMs) {
  sessionActive_ = false;
  sessionStartedAtMs_ = nowMs;
  lastUpdateMs_ = nowMs;
  hasLastUpdate_ = true;
  distanceKm_ = 0.0f;
  fuelLiters_ = 0.0f;
}

void FromStartTrip::startSession(uint32_t nowMs) {
  if (!sessionActive_) {
    sessionActive_ = true;
    sessionStartedAtMs_ = nowMs;
  }
  lastUpdateMs_ = nowMs;
  hasLastUpdate_ = true;
}

void FromStartTrip::integrate(const VehicleState& state) {
  if (!hasLastUpdate_ || state.nowMs <= lastUpdateMs_) {
    return;
  }

  const float hours = static_cast<float>(state.nowMs - lastUpdateMs_) /
                      (1000.0f * 60.0f * 60.0f);
  distanceKm_ += state.speedKph * hours;
  fuelLiters_ += state.fuelRateLitersPerHour * hours;
}

FromStartTripSnapshot FromStartTrip::snapshot(const VehicleState& state) const {
  FromStartTripSnapshot result;
  result.active = sessionActive_;
  result.elapsedMs = sessionActive_ ? state.nowMs - sessionStartedAtMs_ : 0;
  result.distanceKm = distanceKm_;
  result.fuelLiters = fuelLiters_;

  const float elapsedHours =
      static_cast<float>(result.elapsedMs) / (1000.0f * 60.0f * 60.0f);
  if (elapsedHours > 0.0f) {
    result.averageSpeedKph = distanceKm_ / elapsedHours;
  }

  if (distanceKm_ > 0.01f) {
    result.averageConsumptionLPer100Km = (fuelLiters_ / distanceKm_) * 100.0f;
  }

  return result;
}
