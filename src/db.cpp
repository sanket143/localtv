#include "db.h"
#include "util.h"
#include <print>
#include <sqlite3.h>

// 1. The Callback Function
// This function runs automatically FOR EVERY ROW returned by the SELECT query.
int default_callback(void *data, int argc, char **argv, char **azColName) {
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

DB::DB(const char *filename) { sqlite3_open(filename, &conn); }

int DB::run(const char *sql) {
  return sqlite3_exec(conn, sql, &default_callback, nullptr, nullptr);
}

void DB::select(const char *sql, int (*cb)(void *data, int argc, char **argv,
                                           char **azColName)) {

  int test_data[] = {143};
  char *err_msg;
  std::println("here in select");

  int code = sqlite3_exec(conn, sql, cb, &test_data, &err_msg);

  if (code > 0) {
    // Yes we crash if there's an error, be careful
    sqlite3_free(err_msg);
    die(err_msg);
  } else {
    sqlite3_free(err_msg);
  }

  std::println("after  sqlite3_exec: {}", code);
}

void DB::exec(const char *sql,
              int (*cb)(void *data, int argc, char **argv, char **azColName),
              void *data) {
  char *err_msg;

  // I'm getting away from using helpers, because it'll keep getting increased
  // Until I end up with same signature as sqlite3_exec
  int code = sqlite3_exec(conn, sql, cb, &data, &err_msg);

  if (code > 0) {
    // Yes we crash if there's an error, be careful
    sqlite3_free(err_msg);
    die(err_msg);
  } else {
    sqlite3_free(err_msg);
  }
}
