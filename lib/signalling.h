
// extern previous settings

extern pthread_t pt;

void handle_sigint (int param) {
  // fix stdin
  exit (0);
}

void handle_sigterm (int p) {
  handle_sigint (p);
}

void exxit (void) {
  set_old_settings ();
  pthread_cancel (pt);
  printf ("BLEH!\n"); 
}
