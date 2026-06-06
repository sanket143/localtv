#include "db.h"
#include "mpv_helpers.h"
#include <SDL3/SDL.h>
#include <string>

// custom events to trigger sdl redraw
static Uint32 wakeup_on_mpv_render_update, wakeup_on_mpv_events;

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

  static void on_mpv_events(void *ctx);
  static void on_mpv_render_update(void *ctx);
};
