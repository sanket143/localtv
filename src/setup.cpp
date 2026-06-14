#include "db.h"
#include "util.h"
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <print>
#include <pugixml.hpp>

int add_video_details(std::string id, std::string title,
                      std::string description, int duration) {
  DB db(const_cast<char *>("data.db"));

  std::string sql = std::format(
      "insert into video(id, title, description, duration) values "
      "('{}', '{}', '{}', {}) on conflict (id) do update set title = "
      "EXCLUDED.title, duration = EXCLUDED.duration, description = "
      "EXCLUDED.description;",
      id, title, description, duration);

  std::println("{}", sql);

  int resp = db.run(sql.data());
  std::println("{}", resp);
  return 0;
}

void get_video_info(std::string url) {
  std::string cmd = std::format("yt-dlp --print '{{ \"id\": %(id)j, \"title\": "
                                "%(title)j, \"duration\": %(duration)j, "
                                "\"description\": %(description)j }}' {}",
                                url);

  std::println("{}", cmd);
  std::string out = exec_cmd(cmd.data());

  Json::Value root;
  Json::Reader reader;

  reader.parse(out, root);

  add_video_details(root["id"].asString(), root["title"].asString(),
                    root["description"].asString(), root["duration"].asInt());

  std::println("{}", root["id"].asString());
}

int main(int argc, char *argv[]) {
  const char *cmd = "ls";

  std::string out = exec_cmd(cmd);
  std::println("{}", out);

  get_video_info("https://www.youtube.com/watch?v=Mwp-uGCUZE4");

  return 0;
}
