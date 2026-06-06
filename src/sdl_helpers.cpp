#include "sdl_helpers.h"
#include "util.h"

void print_size(SDL_Window *window) {
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  std::println("{} {}", w, h);
}

void s_sdl_init(SDL_Window *&window) {
  SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    die("Could not initialize SDL3");
  }

  window = SDL_CreateWindow("STV", 1000, 500,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  print_size(window);

  if (!window) {
    die("failed to create SDL window");
  }

  SDL_GLContext glcontext = SDL_GL_CreateContext(window);

  if (!glcontext) {
    die("failed to create SDL GL context");
  }
}
