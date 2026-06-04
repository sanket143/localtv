#include "db.h"
#include <fstream>
#include <print>
#include <sstream>

int read_and_execute_sql_file(DB *db, char *filename) {
  std::ifstream migration_file(filename);

  if (!migration_file.is_open()) {
    std::println(stderr, "Error opening the file!");
    return 1;
  }

  std::ostringstream buffer;
  buffer << migration_file.rdbuf();

  std::string content = buffer.str();
  std::println(stdout, "{}", content);

  db->run(content.data());

  return 0;
}

int main() {
  std::string path = "./migrations/0001-migration-file.sql";
  DB db(const_cast<char *>("data.db"));

  read_and_execute_sql_file(&db, path.data());

  return 0;
}
