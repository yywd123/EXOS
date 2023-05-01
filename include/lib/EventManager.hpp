#pragma once

#include <utils.hpp>
#include <lib/guid.h>

namespace EXOS::Utils::EventManager {
  bool registerEvent(GUID eventGuid, Function<void, void*> eventCallback);
  bool notifyEvent(GUID eventGuid, void *params);
}