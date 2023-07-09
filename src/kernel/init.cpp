#include <platform.hpp>
#include <exos/fbcon.hpp>
#include <exos/serial.hpp>

USE(EXOS::Drivers);

void
initializeKernel() {
  Platform::MultiProcessor::initBootstrapProcessor();
}
