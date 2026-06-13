#include "db.h"
#include "util.h"
#include <iostream>
#include <print>
#include <pugixml.hpp>

int helper(int argc, char *argv[]) {
  DB db(const_cast<char *>("data.db"));

  std::string id, title;
  int duration;

  std::getline(std::cin, id);
  std::getline(std::cin, title);
  std::cin >> duration;

  std::println("{} {} {}", id, title, duration);

  // ideally I should sanitize this but keeping it like this for now
  std::string sql = std::format("insert into video(id, title, duration) values "
                                "('{}', '{}', {}) on conflict (id) do nothing;",
                                id, title, duration);

  int resp = db.run(sql.data());
  std::println("{}", resp);
  return 0;
}

int main(int argc, char *argv[]) {
  const char *cmd = "ls";

  // Why is this an issue? I'm including util.h
  // Signatures are also same
  std::string out = exec_cmd(cmd);

  std::println("{}", out);

  return 0;
}
