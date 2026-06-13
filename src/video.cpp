#include "video.h"
#include "db_helper.h"
#include "util.h"
#include <format>

Video::Video(const char *video_id) {
  std::string sql = std::format("select * from video;", video_id);

  just_run_sql(sql.data());
}
