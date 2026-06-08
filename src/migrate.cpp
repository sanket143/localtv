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

int main(int argc, char *argv[]) {
  if (argc > 1 && std::string(argv[1]) == "reset") {
    std::string path = "./migrations/drop.sql";
    DB db(const_cast<char *>("data.db"));

    read_and_execute_sql_file(&db, path.data());
  }

  std::string path = "./migrations/setup.sql";
  DB db(const_cast<char *>("data.db"));

  read_and_execute_sql_file(&db, path.data());

  return 0;
}
