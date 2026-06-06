#include "app.h"
#include <SDL3/SDL.h>
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <print>

int main() {
  App app;

  int i = 0;

  while (app.loop()) {
  }

  std::println("properly terminated");

  return 0;
}
