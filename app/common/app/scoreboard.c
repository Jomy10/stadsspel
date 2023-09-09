#include "include/scoreboard.h"

static struct Scoreboard scoreboard = (struct Scoreboard) {
  .teamCount = 3,
  .teams = (struct Team[]) {
    (struct Team){.name = "abc"},
    (struct Team){.name = "def"},
    (struct Team){.name = "ghi"},
  },
};
struct Scoreboard* getScoreboard(void) {
  return &scoreboard;
}
