#include "video.h"
#include "util.h"
#include <format>
#include <print>
#include <sqlite3.h>

int callback(void *data, int argc, char **argv, char **azColName) {
  // argc = number of columns
  // argv = array of strings representing row values
  // azColName = array of strings representing column names
  //
  int *decoded = (int *)data;

  std::print("{}\n", decoded[0]);
  std::print("{}\n", argc);
  for (int i = 0; i < argc; i++) {
    // SQLite represents values as strings in this callback.
    // Check for NULL values before printing.
    std::print("{}, {}\n", azColName[i], argv[i]);
  }

  return 0; // Return 0 to continue reading next rows. Returning non-zero stops
            // the query.
}

Video::Video(const char *video_id) {
  // I know I should pass aruond a single reference, but
  // I'll keep it like this for now
  sqlite3_open("data.db", &conn);

  char *errmsg = "";
  int return_code =
      sqlite3_exec(conn, "select * from video;", &callback, nullptr, &errmsg);

  if (return_code > 0) {
    std::println("Error running query: {}", errmsg);
  }

  sqlite3_free(errmsg);
}
