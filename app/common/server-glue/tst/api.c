#include "../include/api.h"

struct Scoreboard* fetchScoreboard(void) {
  static struct Scoreboard scoreboard;
  scoreboard = (struct Scoreboard) {
    .teamCount = 3,
    .teams = (struct Team[]) {
      (struct Team){.name = "team 1"},
      (struct Team){.name = "team 2"},
      (struct Team){.name = "team 3"},
    },
  };

  return &scoreboard;
}
