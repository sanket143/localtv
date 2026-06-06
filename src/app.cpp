#include "app.h"
#include "sdl_helpers.h"
#include "util.h"
#include <print>

App::App() : db(const_cast<char *>("data.db")) {
  mpv = mpv_create();
  s_sdl_init(window);
  s_mpv_init(mpv, mpv_gl);
}

App::~App() {
  // Destroy the GL renderer and all of the GL objects it allocated. If video
  // is still running, the video track will be deselected.
  mpv_destroy(mpv);
  mpv_render_context_free(mpv_gl);
}

int App::loop() {
  int redraw = 0;
  SDL_Event event;

  if (SDL_WaitEvent(&event) != 1)
    die("event loop error");

  switch (event.type) {
  case SDL_EVENT_QUIT:
    return 0;

  case SDL_EVENT_WINDOW_EXPOSED:
    redraw = 1;
    break;

  default:
    break;
  }

  if (redraw) {
    std::println("REDRAW");
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
