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
#include <sstream>

enum YT_TYPE { PLAYLIST, VIDEO };

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

YT_TYPE get_type(std::string url) {
  if (url.contains("playlist")) {
    return PLAYLIST;
  }

  return VIDEO;
}

void add_videos_list_from_playlist(Channel *current_channel, std::string url) {
  std::string cmd = std::format("yt-dlp --flat-playlist --get-url '{}'", url);
  std::istringstream out(exec_cmd(cmd.data()));
  std::regex re("^\\s+|\\s+$");

  std::string s;

  while (getline(out, s)) {
    s = std::regex_replace(s, re, "");

    if (s.length() > 0) {
      if (s.size() > 0) {
        std::string video_id = get_video_info(s);
        add_to_schedule(current_channel->id, video_id);
      }
    }
  }
}

void parse_videos() {
  std::ifstream f("videos.txt");
  std::regex re("^\\s+|\\s+$");

  if (!f.is_open()) {
    die("failed to open the file");
  }

  std::string s;
  Channel current_channel;

  while (getline(f, s)) {
    s = std::regex_replace(s, re, "");

    if (s.length() > 0) {
      if (s[0] == '[') {
        current_channel =
            Channel::from_channel_name(s.substr(1, s.length() - 2));

      } else if (s.size() > 0) {
        switch (get_type(s)) {
        case VIDEO: {
          std::string video_id = get_video_info(s);
          add_to_schedule(current_channel.id, video_id);
          break;
        }
        case PLAYLIST: {
          add_videos_list_from_playlist(&current_channel, s);
          break;
        }
        }
      }
    }
  }

  f.close();
}

int main(int argc, char *argv[]) {
  parse_videos();
  return 0;
}
