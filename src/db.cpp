#include "db.h"

void DB::init(const char *filename) { sqlite3_open(filename, &conn); }
void DB::run(const char *sql) {
  sqlite3_exec(conn, sql, nullptr, nullptr, nullptr);
}
