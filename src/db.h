#include <sqlite3.h>

class DB {
private:
  sqlite3 *conn;

public:
  DB(const char *filename);
  int run(const char *sql);
};
