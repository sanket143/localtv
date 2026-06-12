#include "db.h"
#include <print>

// 1. The Callback Function
// This function runs automatically FOR EVERY ROW returned by the SELECT query.
int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  // argc = number of columns
  // argv = array of strings representing row values
  // azColName = array of strings representing column names

  for (int i = 0; i < argc; i++) {
    // SQLite represents values as strings in this callback.
    // Check for NULL values before printing.
    std::print("{}", azColName[i]);
  }

  return 0; // Return 0 to continue reading next rows. Returning non-zero stops
            // the query.
}

DB::DB(const char *filename) { sqlite3_open(filename, &conn); }

int DB::run(const char *sql) {
  return sqlite3_exec(conn, sql, callback, nullptr, nullptr);
}
