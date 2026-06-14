#include "util.h"
#include <chrono>
#include <memory>
#include <print>

void die(const char *msg) {
  std::println(stderr, "{}", msg);
  exit(1);
}

int get_current_secs() {
  // seconds, we'll use that to check, what's the current schedule
  auto t = std::chrono::system_clock::now();
  int epoch = t.time_since_epoch().count() / (1000 * 1000 * 1000);

  return epoch;
}

std::string exec_cmd(const char *cmd) {
  std::array<char, 128> buffer;
  std::string result;

  // what is FILE?
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

  if (!pipe) {
    die("popen() failed");
  }

  // ?????
  while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) !=
         nullptr) {
    result += buffer.data();
  }

  return result;
}
