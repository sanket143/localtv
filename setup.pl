my @videoUrls = split('\n', `cat videos.txt`);

foreach $url (@videoUrls){
  my $videoInfo = `yt-dlp --print id --print title --print duration $url`;
  my @info = split('\n', $videoInfo);

  my $id = @info[0];
  my $title = @info[1];
  my $duration = @info[2];

  my $info = "$id\n$title\n$duration";
  my $build = `cmake -B build && cmake --build build`;
  my $output = `echo "$info" | ./build/localtv_setup`;

  print "$output";

  sleep(1);
}
