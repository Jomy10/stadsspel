#ifndef _SCOREBOARD_H
#define _SCOREBOARD_H

struct Team {
  char* name;
};

struct Scoreboard {
  int teamCount;
  struct Team* teams;
};

// TODO: API call
struct Scoreboard* getScoreboard();

#endif
