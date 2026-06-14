#include "db.h"
#include "video.h"
#include <tuple>
#include <vector>

class Channel {
public:
  int id;
  std::string name;
  std::vector<Video> videos;

  Channel();
  Channel(int id);

  static Channel from_channel_name(std::string channel_name);
  std::tuple<Video, int> get_current_video();

  void process_schedule(DB *db);
};
