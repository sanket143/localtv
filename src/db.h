#pragma once

#include <sqlite3.h>

class DB {
private:
  sqlite3 *conn;

public:
  DB(const char *filename);
  int run(const char *sql);
  void select(const char *sql,
              int (*cb)(void *data, int argc, char **argv, char **azColName));
  void exec(const char *sql,
            int (*cb)(void *data, int argc, char **argv, char **azColName),
            void *);
};

DB get_db_instance();
