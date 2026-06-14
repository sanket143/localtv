#include "channel.h"
#include "db.h"
#include "schedule.h"
#include "util.h"
#include <fstream>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <print>
#include <pugixml.hpp>
#include <regex>

int add_video_details(std::string id, std::string title,
                      std::string description, int duration) {
  DB db(const_cast<char *>("data.db"));

  std::regex re("'");
  std::string sql = std::format(
      "insert into video(id, title, description, duration) values "
      "('{}', '{}', null, {}) on conflict (id) do update set title = "
      "EXCLUDED.title, duration = EXCLUDED.duration, description = "
      "EXCLUDED.description;",
      id, std::regex_replace(title, re, "''"), duration);

  std::println("{}", sql);

  int resp = db.run(sql.data());
  std::println("{}", resp);
  return 0;
}

std::string get_video_info(std::string url) {
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

  return root["id"].asString();
}

void parse_videos() {
  std::ifstream f("videos.txt");

  if (!f.is_open()) {
    die("failed to open the file");
  }

  std::string s;
  Channel current_channel;

  while (getline(f, s)) {
    if (s.length() > 0) {
      if (s[0] == '[') {
        current_channel =
            Channel::from_channel_name(s.substr(1, s.length() - 2));

      } else {
        std::string video_id = get_video_info(s);

        add_to_schedule(current_channel.id, video_id);
      }
    }
  }

  f.close();
}

int main(int argc, char *argv[]) {
  parse_videos();
  return 0;
}
