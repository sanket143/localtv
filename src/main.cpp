#include <mpv/client.h>
#include <print>

#define MPV_REPLY_PROPERTY 2

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
  mpv_set_option_string(
      ctx, "audio-client-name",
      "baka-mplayer"); // show correct icon in e.g. pavucontrol

  if (mpv_initialize(ctx) < 0) {
    std::println("Could not initialize mpv");
  }

  std::println("Success: mpv context created");

  return 0;
}
