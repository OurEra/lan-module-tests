#ifndef PLAY_EV_WRAP_H
#define PLAY_EV_WRAP_H
#include <ev.h>
EV_P = NULL;
void wrapev_init() {
  // Create our single-loop for this single-thread application
  loop = ev_default_loop(0);
}

void wrapev_deinit() {
  ev_loop_destroy(loop);
}

#endif // PLAY_EV_WRAP_H
