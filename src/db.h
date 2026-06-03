#include <sqlite3.h>

class DB {
private:
  sqlite3 *conn;

public:
  void init(const char *filename);
  void run(const char *sql);
};
