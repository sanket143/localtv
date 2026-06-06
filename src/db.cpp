#include "db.h"

DB::DB(const char *filename) { sqlite3_open(filename, &conn); }

int DB::run(const char *sql) {
  return sqlite3_exec(conn, sql, nullptr, nullptr, nullptr);
}
