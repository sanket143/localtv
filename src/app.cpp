#include "app.h"
#include "mpv_helpers.h"
#include "util.h"
#include "video.h"
#include <GL/gl.h>
#include <SDL3/SDL.h>
#include <mpv/client.h>
#include <print>
#include <stdlib.h>
#include <tuple>
#include <unistd.h>

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
}

App::~App() {
  // Destroy the GL renderer and all of the GL objects it allocated. If video
  // is still running, the video track will be deselected.
  mpv_render_context_free(mpv_gl);
  mpv_destroy(mpv);
}

int App::loop() {
  int should_redraw = 0;
  SDL_Event event;

  if (SDL_WaitEvent(&event) != 1) {
    die("event loop error");
  }

  switch (event.type) {
  case SDL_EVENT_QUIT:
    return 0;

  case SDL_EVENT_WINDOW_EXPOSED:
    should_redraw = 1;
    break;

  case SDL_EVENT_KEY_DOWN:
    if (event.key.scancode == SDL_SCANCODE_RETURN) {
      play_channel();
    }

    if (event.key.scancode == SDL_SCANCODE_J) {
      next_channel();
    }
    if (event.key.scancode == SDL_SCANCODE_K) {
      prev_channel();
    }

    break;

  default:
    if (event.type == wakeup_on_mpv_render_update) {
      // I'm copying, I've no idea what this does
      uint64_t flags = mpv_render_context_update(mpv_gl);

      if (flags & MPV_RENDER_UPDATE_FRAME) {
        should_redraw = 1;
      }
    }

    break;
  }

  if (should_redraw) {
    redraw();
  }

  return 1;
}

void App::play(std::string filepath, int start_seconds) {
  std::string start_opt = "start=" + std::to_string(start_seconds);
  const char *cmd[] = {"loadfile", filepath.data(),  "replace",
                       "0",        start_opt.data(), NULL};

  mpv_command_async(mpv, 0, cmd);
}

void App::seek(int duration) {
  std::println("{}", duration);

  std::string duration_str = std::to_string(duration);
  const char *cmd[] = {"seek", duration_str.data(), "absolute", NULL};
  mpv_command(mpv, cmd);
}

void App::draw_overlay() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(1.0, 1.0, 1.0, 0.5);

  glBegin(GL_QUADS);
  glVertex2f(0.8f, 0.1f);
  glVertex2f(0.9f, 0.1f);
  glVertex2f(0.9f, 0.2f);
  glVertex2f(0.8f, 0.2f);
  glEnd();
}

void App::redraw() {
  int w, h;
  SDL_GetWindowSizeInPixels(window, &w, &h);
  mpv_opengl_fbo opengl_fbo = {
      .fbo = 0,
      .w = w,
      .h = h,
  };

  mpv_render_param params[] = {
      {MPV_RENDER_PARAM_OPENGL_FBO, &opengl_fbo},
      {MPV_RENDER_PARAM_FLIP_Y, &mpv_true},
      {MPV_RENDER_PARAM_INVALID, nullptr},
  };

  mpv_render_context_render(mpv_gl, params);
  glViewport(0, 0, w, h);

  draw_overlay();

  SDL_GL_SwapWindow(window);
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
  auto cb = [](void *data, int argc, char **argv, char **column_name_map) {
    auto *channels_data = static_cast<std::vector<Channel> *>(data);

    for (int i = 0; i < argc; i++) {
      if (strcmp(column_name_map[i], "id") == 0) {
        channels_data->push_back(Channel(std::stoi(argv[i])));
      }
    }

    return 0;
  };

  db.exec("select * from channel;", cb, &channels);
}

void App::next_channel() {
  current_channel = (current_channel + 1) % channels.size();
  play_channel();
}

void App::prev_channel() {
  current_channel = (current_channel - 1 + channels.size()) % channels.size();
  play_channel();
}

void App::play_channel() {
  channels[current_channel].process_schedule(&db);

  if (channels[current_channel].videos.size() > 0) {

    std::tuple<Video, int> play_info =
        channels[current_channel].get_current_video();

    Video video = std::get<0>(play_info);
    int seek_duration = std::get<1>(play_info);

    play(video.url(), seek_duration);
  }
}
