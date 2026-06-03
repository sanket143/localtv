#include "db.h"
#include <filesystem>
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
  std::string path = "./migrations";

  DB db(const_cast<char *>("data.db"));

  try {
    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(path)) {
      read_and_execute_sql_file(&db, entry.path().string().data());
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::println(stderr, "{}", e.what());
  }

  return 0;
}
