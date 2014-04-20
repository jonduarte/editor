#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <editline/readline.h>

static int running = 1;

void exit_handler(int _val)
{
  running = 0;
}

int main(int argc, char *argv[])
{

  signal(SIGINT, exit_handler);

  if (argc < 2) {
    fprintf(stderr, "Usage:\n\t./editor filename\n");
    exit(1);
  }

  FILE *handler = fopen(argv[1], "a");

  if (handler == NULL) {
    fprintf(stderr, "Error when opening the file!\n");
    exit(1);
  }

  while(running) {
    char *input = readline("");
    add_history(input);
    fprintf(handler, "%s", input);
    free(input);
  }

  fclose(handler);
  return 0;
}
