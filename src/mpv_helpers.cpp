#include "mpv_helpers.h"
#include "util.h"
#include <print>

static void *get_proc_address_mpv(void *fn_ctx, const char *name) {
  (void)fn_ctx;

  return reinterpret_cast<void *>(SDL_GL_GetProcAddress(name));
}

void s_mpv_init(mpv_handle *&mpv, mpv_render_context *&mpv_gl,
                SDL_Window *&window) {

  mpv_set_option_string(mpv, "vo", "libmpv");
  if (mpv_initialize(mpv) < 0) {
    die("Could not initialize mpv");
  }

  mpv_request_log_messages(mpv, "debug");

  SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    die("SDL init failed");
  }

  window = SDL_CreateWindow("STV", 1000, 500,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!window) {
    die("failed to create SDL window");
  }

  SDL_GLContext glcontext = SDL_GL_CreateContext(window);

  if (!glcontext) {
    die("failed to create SDL GL context");
  }

  // mpv_set_option_string(mpv, "input-cursor", "no"); // no mouse handling
  // mpv_set_option_string(mpv, "cursor-autohide",
  //                       "no"); // no cursor-autohide, we handle that
  // mpv_set_option_string(mpv, "ytdl", "yes"); // youtube-dl support
  // mpv_set_option_string(mpv, "sub-auto",
  //                       "fuzzy"); // Automatic subfile detection
  // mpv_set_option_string(mpv, "audio-client-name",
  //                       "stv"); // show correct icon in e.g. pavucontrol

  mpv_opengl_init_params gl_init_params;
  gl_init_params.get_proc_address = get_proc_address_mpv;

  mpv_render_param params[] = {
      {MPV_RENDER_PARAM_API_TYPE,
       const_cast<char *>(MPV_RENDER_API_TYPE_OPENGL)},
      {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params},
      // Tell libmpv that you will call mpv_render_context_update() on render
      // context update callbacks, and that you will _not_ block on the core
      // ever (see <libmpv/render.h> "Threading" section for what libmpv
      // functions you can call at all when this is active).
      // In particular, this means you must call e.g. mpv_command_async()
      // instead of mpv_command().
      // If you want to use synchronous calls, either make them on a separate
      // thread, or remove the option below (this will disable features like
      // DR and is not recommended anyway).
      {MPV_RENDER_PARAM_ADVANCED_CONTROL, &mpv_true},
      {MPV_RENDER_PARAM_INVALID, nullptr},
  };

  // This makes mpv use the currently set GL context. It will use the callback
  // (passed via params) to resolve GL builtin functions, as well as extensions.
  if (mpv_render_context_create(&mpv_gl, mpv, params) < 0) {
    die("failed to initialize mpv GL context");
  } else {
    std::println("Initialized mpv_renderer");
  }
}
