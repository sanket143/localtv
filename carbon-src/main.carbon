import Cpp library "./std/print.h";
import Cpp library "<cstdio>";
import Cpp library "<SDL3/SDL.h>";

fn Run() {
  Cpp.SDL_Window window;
  Cpp.puts(Cpp.std.data("hello world"));
}
