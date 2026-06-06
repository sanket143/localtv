#include <print>

static void die(const char *msg) {
  std::print(stderr, "%s\n", msg);
  exit(1);
}
