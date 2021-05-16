#include <Arduino.h>
#include "State.h"

State::State() {}

State::~State() {}

State* State::Instance() {
  return NULL;
}

void State::DoUpdate() {}

void State::DoRender() {}

void State::DoInput() {}

void State::OnLostDevice() {}

void State::OnResetDevice() {}

void State::OnLoadState() {}

void State::OnLeaveState() {}
