#include "schedule.h"
#include "db.h"
#include <format>
#include <print>

void add_to_schedule(int channel_id, std::string video_id) {
  DB db = get_db_instance();

  std::string sql = std::format(
      "insert into schedule(fk_channel_id, fk_video_id) values "
      "({}, '{}') on conflict (fk_channel_id, fk_video_id) do nothing;",
      channel_id, video_id);

  std::println("Added {} in {}", video_id, channel_id);
  db.exec(sql.data(), nullptr, nullptr);
}
