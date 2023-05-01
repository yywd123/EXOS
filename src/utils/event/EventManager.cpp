#include <utils.hpp>
#include <lib/guid.h>
#include <lib/LinkedList.hpp>

using namespace EXOS::Utils;

class EventInfo {
public:
  GUID eventGuid;
  Function<void, void*> eventCallback;
  EventInfo(GUID guid, Function<void, void*> callback) : eventGuid(guid), eventCallback(callback) {}

  bool matchGuid(GUID guid) {
    return (GuidCompare(this->eventGuid, guid));
  }
};

static AbstractList<EventInfo*> eventList = nullptr;

namespace EXOS::Utils::EventManager {
  void registerEvent(GUID eventGuid, Function<void, void*> eventCallback) {
    if (eventList == nullptr) eventList = new LinkedList<EventInfo*>();
    eventList->add(new EventInfo(eventGuid, eventCallback));
  }

  bool notifyEvent(GUID eventGuid, void *params) {
    bool isCalled = false;
    iter(eventList->getSize()) {
      EventInfo *info = eventList->get(i);
      if (info->matchGuid(eventGuid)) {
        info->eventCallback(params);
        isCalled = true;
      }
    }
    return isCalled;
  }
}