#include "channel.h"

Channel::Channel() {};
Channel::Channel(const std::string id) : id(id) {
  // populate videos from db
  // compute the schedule, and current video index
  //
  // E(s + 1 min break)
  // Need some time to think about this
}

Video Channel::get_current_video() {
  // I think we should compute here, instead of in the constructor
}
