#include "db.h"
#include "video.h"

class Channel {
private:
  DB *db;

  Video *videos;

public:
  int id;

  Channel(int id);
  Video get_current_video();
};
