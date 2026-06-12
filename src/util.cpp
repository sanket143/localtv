#include "util.h"
#include <chrono>
#include <print>

void die(const char *msg) {
  std::print(stderr, "%s\n", msg);
  exit(1);
}

int get_current_secs() {
  // seconds, we'll use that to check, what's the current schedule
  auto t = std::chrono::system_clock::now();
  int epoch = t.time_since_epoch().count() / (1000 * 1000 * 1000);

  return epoch;
}
