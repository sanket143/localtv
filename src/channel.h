#include "db.h"
#include "video.h"

class Channel {
private:
  DB *db;

  Video *videos;

public:
  std::string id;

  Channel();
  Channel(std::string id);
  Video get_current_video();
};
