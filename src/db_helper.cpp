#include "db_helper.h"

int just_run_sql(const char *sql) {
  DB db("data.db");
  return db.select(sql, &default_callback);
}
