#pragma once

#include "FsmComponentId.h"

class BaseFsmComponent
{
  public:
    virtual void update() {}
};

/**
 * Base class for FsmComponent
 */
template <typename Derived> class FsmComponent : public BaseFsmComponent
{
  public:
    static FsmComponentId getClassId() {
      static FsmComponentId id = FsmComponentIdGenerator::nextId++;
      return id;
    }
};
