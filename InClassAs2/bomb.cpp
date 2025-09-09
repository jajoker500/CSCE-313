void phase_1(char *s) {
  // ....snipped...
  if (strcmp (input, s) != 0) {
      printf ("Nope! Please guess again.\n");
      exit (1) ;
  }
}

int main(int argc, char **argv) {
  phase_1(argv[1]);
  printf ("You got it!\n");
}