#include "app.h"
#include "mpv_helpers.h"
#include "util.h"
#include "video.h"
#include <print>
#include <stdlib.h>

App::App() : db("data.db"), current_channel(0) {
  mpv = mpv_create();
  s_mpv_init(mpv, mpv_gl, window);

  wakeup_on_mpv_render_update = SDL_RegisterEvents(1);
  wakeup_on_mpv_events = SDL_RegisterEvents(1);

  if (wakeup_on_mpv_render_update == (Uint32)-1 ||
      wakeup_on_mpv_events == (Uint32)-1) {
    die("could not register events");
  }

  mpv_set_wakeup_callback(mpv, on_mpv_events, NULL);
  mpv_render_context_set_update_callback(mpv_gl, on_mpv_render_update, NULL);

  load_channels();
  // TODO: load all the channels here
}

App::~App() {
  // Destroy the GL renderer and all of the GL objects it allocated. If video
  // is still running, the video track will be deselected.
  mpv_render_context_free(mpv_gl);
  mpv_destroy(mpv);
}

int App::loop() {
  int redraw = 0;
  SDL_Event event;

  if (SDL_WaitEvent(&event) != 1) {
    die("event loop error");
  }

  switch (event.type) {
  case SDL_EVENT_QUIT:
    return 0;

  case SDL_EVENT_WINDOW_EXPOSED:
    redraw = 1;
    break;

  case SDL_EVENT_KEY_DOWN:
    if (event.key.scancode == SDL_SCANCODE_RETURN) {
      std::println("Here, trying to play, or get info");
      Video video("jjp3WC8Unj8");
      // play("https://www.youtube.com/watch?v=jjp3WC8Unj8");
    }

    break;

  default:
    if (event.type == wakeup_on_mpv_render_update) {
      // I'm copying, I've no idea what this does
      uint64_t flags = mpv_render_context_update(mpv_gl);

      if (flags & MPV_RENDER_UPDATE_FRAME) {
        redraw = 1;
      }
    }

    break;
  }

  if (redraw) {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    mpv_opengl_fbo opengl_fbo = {
        .fbo = 0,
        .w = w,
        .h = h,
    };

    mpv_render_param params[] = {
        // Specify the default framebuffer (0) as target. This will
        // render onto the entire screen. If you want to show the video
        // in a smaller rectangle or apply fancy transformations, you'll
        // need to render into a separate FBO and draw it manually.
        {MPV_RENDER_PARAM_OPENGL_FBO, &opengl_fbo},
        // Flip rendering (needed due to flipped GL coordinate system).
        {MPV_RENDER_PARAM_FLIP_Y, &mpv_true},
        {MPV_RENDER_PARAM_INVALID, nullptr},
    };

    // See render_gl.h on what OpenGL environment mpv expects, and
    // other API details.
    mpv_render_context_render(mpv_gl, params);
    SDL_GL_SwapWindow(window);
  }

  return 1;
}

void App::play(std::string filepath) {
  const char *cmd[] = {"loadfile", filepath.data(), NULL};
  mpv_command_async(mpv, 0, cmd);
}

void App::on_mpv_events(void *ctx) {
  SDL_Event event = {.type = wakeup_on_mpv_events};
  SDL_PushEvent(&event);
}

void App::on_mpv_render_update(void *ctx) {
  SDL_Event event = {.type = wakeup_on_mpv_render_update};
  SDL_PushEvent(&event);
}

// Channel related functions
void App::load_channels() {
  Channel *channels_ptr = new Channel[5];

  std::println("println");
  channels_ptr[0] = Channel("UCOxqgCwgOqC2lMqC5PYz_Dg");
  std::println("channels_ptr {}", channels_ptr[0].id);

  channels.push_back(Channel("UCOxqgCwgOqC2lMqC5PYz_Dg"));

  auto cb = [](void *data, int argc, char **argv, char **azColName) {
    std::print("Inside pointer address: {}\n", data);
    // std::vector<Channel> *channels_data = (std::vector<Channel> *)data;
    Channel *channels_data = (Channel *)data;

    // channels->push_back(Channel("test"));

    // why isn't this size value same as the outside?
    // it's printing something random
    // std::print("Inside size: {}\n", channels_data->size());
    std::print("Channel: [{}]", channels_data[0].id);

    // for (int i = 0; i < argc; i++) {
    //   std::print("{}, {}\n", azColName[i], argv[i]);
    // }

    return 0;
  };

  std::vector<Channel> *ptr = &channels;
  std::print("Outside pointer address: {}\n", static_cast<const void *>(ptr));
  std::print("Outside size: {}\n", ptr->size());

  db.exec("select * from channel;", cb, channels_ptr);

  std::print("Outside size: {}\n", ptr->size());
}

void App::next_channel() {}
void App::prev_channel() {}
void App::play_channel(int channel_id) {
  db.select("select * from schedule limit 1;", &default_callback);
}
