#include <SDL3/SDL.h>
#include <mpv/client.h>
#include <mpv/render_gl.h>

// I just copied these from
// https://github.com/mpv-player/mpv-examples/blob/master/libmpv/sdl/main.c
static Uint32 wakeup_on_mpv_render_update, wakeup_on_mpv_events;
static int mpv_true = 1;
void s_mpv_init(mpv_handle *&mpv, mpv_render_context *&mpv_gl);
