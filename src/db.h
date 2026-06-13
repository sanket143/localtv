#pragma once

#include <sqlite3.h>

int default_callback(void *data, int argc, char **argv, char **azColName);

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
