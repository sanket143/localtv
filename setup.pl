my @videoUrls = split('\n', `cat videos.txt`);

foreach $url (@videoUrls){
  my $videoInfo = `yt-dlp --print id --print title --print duration --print description --print channelId $url`;
  my @info = split('\n', $videoInfo);

  my $id = @info[0];
  my $title = @info[1];
  my $duration = @info[2];

  my $info = "$id\n$title\n$duration\n@info[3]\n@info[4]\n";
  # my $build = `cmake -B build && cmake --build build`;
  # my $output = `echo "$info" | ./build/localtv_setup`;

  print "$info";

  sleep(1);
}
