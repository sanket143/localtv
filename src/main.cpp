#include "app.h"
#include <print>

int main() {
  App app;

  int i = 0;

  while (app.loop()) {
  }

  std::println("properly terminated");

  return 0;
}
