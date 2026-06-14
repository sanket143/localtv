#include "db.h"
#include "video.h"
#include <tuple>
#include <vector>

class Channel {
private:
  int id;

  std::vector<Video> videos;

public:
  Channel(int id);
  std::tuple<Video, int> get_current_video();
  void process_schedule(DB *db);
};
