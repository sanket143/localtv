#include "db.h"
#include <fstream>
#include <print>
#include <sstream>

int main() {
  DB db;
  db.init(const_cast<char *>("data.db"));

  std::ifstream migration_file("./migrations/0001-create-channels-table.sql");

  if (!migration_file.is_open()) {
    std::println(stderr, "Error opening the file!");
    return 1;
  }

  std::ostringstream buffer;
  buffer << migration_file.rdbuf();

  std::string content = buffer.str();

  std::println(stdout, "{}", content);

  db.run(content.data());

  return 0;
}
