#ifndef _APP_API_H
#define _APP_API_H

struct Team {
  char* name;
};

struct Scoreboard {
  int teamCount;
  struct Team* teams;
};

struct Scoreboard* fetchScoreboard(void);

#endif
