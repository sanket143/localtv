#include "db.h"
#include "util.h"
#include <print>
#include <sqlite3.h>

DB::DB(const char *filename) { sqlite3_open(filename, &conn); }

int DB::run(const char *sql) {
  char *err_msg;
  int code = sqlite3_exec(conn, sql, nullptr, nullptr, &err_msg);

  if (code > 0) {
    // Yes we crash if there's an error, be careful
    sqlite3_free(err_msg);
    die(err_msg);
  } else {
    sqlite3_free(err_msg);
  }

  return code;
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

  int code = sqlite3_exec(conn, sql, cb, data, &err_msg);

  if (code > 0) {
    // Yes we crash if there's an error, be careful
    sqlite3_free(err_msg);
    die(err_msg);
  } else {
    sqlite3_free(err_msg);
  }
}

DB get_db_instance() {
  DB db("data.db");

  return db;
}
