#include "channel.h"
#include "db.h"
#include <SDL3/SDL.h>
#include <mpv/client.h>
#include <mpv/render.h>
#include <mpv/render_gl.h>
#include <string>
#include <vector>

// custom events to trigger sdl redraw
static Uint32 wakeup_on_mpv_render_update, wakeup_on_mpv_events;

class App {
private:
  DB db;

  int epoch;
  SDL_Window *window;

  int current_channel = 0;
  std::vector<Channel> channels;

  // handlers
  mpv_handle *mpv;
  mpv_render_context *mpv_gl;

public:
  App();
  ~App();

  int loop();

  // video options
  void play(std::string filepath, int start_seconds = 0);
  void play_channel();
  void seek(int duration);

  // channel related helpers I guess?
  void load_channels();
  void next_channel();
  void prev_channel();

  // painters
  void draw_overlay();
  void redraw();

  // event listeners
  static void on_mpv_events(void *ctx);
  static void on_mpv_render_update(void *ctx);
};
