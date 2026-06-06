#include "db.h"
#include "mpv_helpers.h"
#include <SDL3/SDL.h>

class App {
private:
  DB db;

  SDL_Window *window;
  mpv_handle *mpv;
  mpv_render_context *mpv_gl;

public:
  App();
  ~App();

  int loop();
};
