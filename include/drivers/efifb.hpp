#pragma once

#include <attribute.h>
#include <display/displayImpl.hpp>

namespace EXOS::Drivers::EfiFb {
  using namespace EXOS::Display;

  DisplayImpl __INIT
  *getDisplayImpl();
}