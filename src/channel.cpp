#include "channel.h"
#include "util.h"
#include <format>
#include <print>
#include <string.h>
#include <tuple>

Channel::Channel(int id) : id(id) {
  // I need a db reference here. Or I can write a separate function for this
  // populate videos from db
  // compute the schedule, and current video index
  //
  // E(s + 1 min break)
  // Need some time to think about this
}

Channel::Channel() {}

Channel Channel::from_channel_name(std::string channel_name) {
  DB db = get_db_instance();
  Channel channel;

  auto cb = [](void *data, int argc, char **argv, char **column_name_map) {
    Channel *channel = (Channel *)data;

    for (int i = 0; i < argc; i++) {
      if (argv[i] != nullptr) {
        if (strcmp(column_name_map[i], "id") == 0) {
          channel->id = std::stoi(argv[i]);
        }

        if (strcmp(column_name_map[i], "name") == 0) {
          channel->name = argv[i];
        }
      }
    }

    return 0;
  };

  std::string sql = std::format(
      "select * from channel as c where c.name = '{}'", channel_name);

  db.exec(sql.data(), cb, &channel);

  return channel;
}

std::tuple<Video, int> Channel::get_current_video() {
  int channel_runtime = 0;

  for (auto video : videos) {
    channel_runtime += video.duration;
  }

  int secs_since_the_channel_started = get_current_secs() % channel_runtime;

  for (auto video : videos) {
    secs_since_the_channel_started -= video.duration;

    if (secs_since_the_channel_started < 0) {
      int seek_value = video.duration + secs_since_the_channel_started;

      std::println("{}, {}, {}", video.duration, seek_value,
                   secs_since_the_channel_started);
      return std::make_tuple(video, seek_value);
    }
  }

  // If no match, start from beginning i guess. Ideally, we should never be
  // here.
  return std::make_tuple(videos[0], 0);
}

void Channel::process_schedule(DB *db) {
  videos.clear();

  auto cb = [](void *data, int argc, char **argv, char **column_name_map) {
    auto *videos_data = static_cast<std::vector<Video> *>(data);

    Video video;

    for (int i = 0; i < argc; i++) {
      if (argv[i] == nullptr) {
        continue;
      }

      if (strcmp(column_name_map[i], "id") == 0) {
        video.id = argv[i];
      }

      if (strcmp(column_name_map[i], "title") == 0) {
        video.title = argv[i];
      }

      if (strcmp(column_name_map[i], "description") == 0) {
        video.description = argv[i];
      }

      if (strcmp(column_name_map[i], "duration") == 0) {
        video.duration = std::stoi(argv[i]);
      }
    }

    videos_data->push_back(video);

    return 0;
  };

  // TODO; Use sqlite3_bind to bind variables
  db->exec(std::format(
               "select v.* from schedule as s join video as v on v.id = "
               "s.fk_video_id where s.fk_channel_id = {} order by created_at;",
               id)
               .data(),
           cb, &videos);

  std::println("process: {}", videos.size());
}
