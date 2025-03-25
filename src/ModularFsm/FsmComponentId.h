#pragma once
using FsmComponentId = int;

const FsmComponentId INVALID_FSM_CLASS_ID = -1;

struct FsmComponentIdGenerator {
  static FsmComponentId nextId;
};