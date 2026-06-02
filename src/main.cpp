#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_init.h>
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <print>

#define MPV_REPLY_PROPERTY 2

static int mpv_true = 1;
static Uint32 wakeup_on_mpv_render_update, wakeup_on_mpv_events;
static void die(const char *msg) {
  std::print(stderr, "%s\n", msg);
  exit(1);
}

static void on_mpv_events(void *ctx) {
  SDL_Event event = {.type = wakeup_on_mpv_events};
  SDL_PushEvent(&event);
}

static void on_mpv_render_update(void *ctx) {
  SDL_Event event = {.type = wakeup_on_mpv_render_update};
  SDL_PushEvent(&event);
}

static void *get_proc_address_mpv(void *fn_ctx, const char *name) {
  (void)fn_ctx;

  return reinterpret_cast<void *>(SDL_GL_GetProcAddress(name));
}

int main() {
  mpv_handle *ctx = mpv_create();

  if (!ctx) {
    std::println("Error: Failed to create mpv context");
  }

  mpv_set_option_string(ctx, "input-cursor", "no"); // no mouse handling
  mpv_set_option_string(ctx, "cursor-autohide",
                        "no"); // no cursor-autohide, we handle that
  mpv_set_option_string(ctx, "ytdl", "yes"); // youtube-dl support
  mpv_set_option_string(ctx, "sub-auto",
                        "fuzzy"); // Automatic subfile detection
  mpv_set_option_string(ctx, "audio-client-name",
                        "stv"); // show correct icon in e.g. pavucontrol
  mpv_set_option_string(ctx, "vo", "libmpv");
  mpv_request_log_messages(ctx, "debug");

  if (mpv_initialize(ctx) < 0) {
    std::println("Could not initialize mpv");
  }

  SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    die("Could not initialize SDL3");
  }

  SDL_Window *window = SDL_CreateWindow(
      "hi", 1000, 500, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!window)
    die("failed to create SDL window");

  SDL_GLContext glcontext = SDL_GL_CreateContext(window);
  if (!glcontext)
    die("failed to create SDL GL context");

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
      {MPV_RENDER_PARAM_INVALID, nullptr}};

  // This makes mpv use the currently set GL context. It will use the callback
  // (passed via params) to resolve GL builtin functions, as well as extensions.
  mpv_render_context *mpv_gl;

  std::println("Hello");

  if (mpv_render_context_create(&mpv_gl, ctx, params) < 0)
    die("failed to initialize mpv GL context");

  std::println("WOrld");

  // We use events for thread-safe notification of the SDL main loop.
  // Generally, the wakeup callbacks (set further below) should do as least
  // work as possible, and merely wake up another thread to do actual work.
  // On SDL, waking up the mainloop is the ideal course of action. SDL's
  // SDL_PushEvent() is thread-safe, so we use that.
  wakeup_on_mpv_render_update = SDL_RegisterEvents(1);
  wakeup_on_mpv_events = SDL_RegisterEvents(1);
  if (wakeup_on_mpv_render_update == (Uint32)-1 ||
      wakeup_on_mpv_events == (Uint32)-1)
    die("could not register events");

  // When normal mpv events are available.
  mpv_set_wakeup_callback(ctx, on_mpv_events, NULL);

  // When there is a need to call mpv_render_context_update(), which can
  // request a new frame to be rendered.
  // (Separate from the normal event handling mechanism for the sake of
  //  users which run OpenGL on a different thread.)
  mpv_render_context_set_update_callback(mpv_gl, on_mpv_render_update, NULL);

  // Play this file.
  const char *cmd[] = {"loadfile",
                       "https://www.youtube.com/watch?v=iqddnwKF8HQ", NULL};

  mpv_command_async(ctx, 0, cmd);

  std::println("Success: mpv context created");

  while (1) {
    SDL_Event event;
    if (SDL_WaitEvent(&event) != 1)
      die("event loop error");

    int redraw = 0;
    switch (event.type) {
    case SDL_EVENT_QUIT:
      goto done;
    case SDL_EVENT_WINDOW_EXPOSED:
      redraw = 1;
      break;
    default:
      // Happens when there is new work for the render thread (such as
      // rendering a new video frame or redrawing it).
      if (event.type == wakeup_on_mpv_render_update) {
        uint64_t flags = mpv_render_context_update(mpv_gl);
        if (flags & MPV_RENDER_UPDATE_FRAME)
          redraw = 1;
      }
      // Happens when at least 1 new event is in the mpv event queue.
      if (event.type == wakeup_on_mpv_events) {
        // Handle all remaining mpv events.
        while (1) {
          mpv_event *mp_event = mpv_wait_event(ctx, 0);
          if (mp_event->event_id == MPV_EVENT_NONE)
            break;
          if (mp_event->event_id == MPV_EVENT_LOG_MESSAGE) {
            continue;
          }
          printf("event: %s\n", mpv_event_name(mp_event->event_id));
        }
      }
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
          {MPV_RENDER_PARAM_INVALID, nullptr}};
      // See render_gl.h on what OpenGL environment mpv expects, and
      // other API details.
      //
      mpv_render_context_render(mpv_gl, params);
      SDL_GL_SwapWindow(window);
    }
  }
done:

  // Destroy the GL renderer and all of the GL objects it allocated. If video
  // is still running, the video track will be deselected.
  mpv_render_context_free(mpv_gl);

  mpv_destroy(ctx);

  printf("properly terminated\n");

  return 0;
}
