#pragma once

#include <sqlite3.h>
#include <string>

class Video {
private:
  // what even is the difference between std::string and char*
  std::string id;
  std::string title; // fulltitle
  std::string description;
  std::string channel_id;
  int duration;    // in epoch
  int upload_date; // just in case, might matter while scheduling

  // I hope opening mutliple sqlite3 doesn't cause any issues
  sqlite3 *conn;

public:
  // First check in the db, if it exists there, get data from there, else
  // We get video info using yt-dlp cmd_exec
  // Parse video info
  // Populate the fields
  // store the video details in data.db
  Video(const char *video_id);
};
