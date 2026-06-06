#include <SDL3/SDL.h>
#include <mpv/client.h>
#include <mpv/render_gl.h>

static int mpv_true = 1;
void s_mpv_init(mpv_handle *&mpv, mpv_render_context *&mpv_gl,
                SDL_Window *&window);
