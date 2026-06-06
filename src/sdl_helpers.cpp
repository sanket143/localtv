#include "sdl_helpers.h"
#include "util.h"

void s_sdl_init(SDL_Window *&window) {
  window = SDL_CreateWindow("STV", 1000, 500,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!window) {
    die("failed to create SDL window");
  }

  SDL_GLContext glcontext = SDL_GL_CreateContext(window);

  if (!glcontext) {
    die("failed to create SDL GL context");
  }
}
