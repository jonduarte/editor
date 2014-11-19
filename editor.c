#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

typedef struct line {
  char contents[80];
  int line_num;
  struct line *next;
} line_t;

void print_buffer(line_t *buffer) {
  if(buffer == NULL)
    return;

  fprintf(stdout, "%3d %s", buffer->line_num, buffer->contents);
  print_buffer(buffer->next);
}

int main(int argc, char *argv[])
{

  if (argc < 2) {
    fprintf(stderr, "Usage:\n\t%s filename\n", argv[0]);
    exit(1);
  }

  FILE *handler = fopen(argv[1], "r");

  if (handler == NULL) {
    fprintf(stderr, "Error when opening the file!\n");
    exit(1);
  }

  line_t *head, *current, *last;
  head = NULL;
  last = NULL;

  char line[80];
  int line_num = 0;

  while(fgets(line, sizeof(line), handler) != NULL) {
    current = (line_t *)malloc(sizeof(line_t));
    strcpy(current->contents, line);
    current->line_num = line_num++;
    current->next = NULL;

    if(head == NULL)
      head = current;

    if(last != NULL)
      last->next = current;

    last = current;
  }

  fclose(handler);
  print_buffer(head);

  return 0;
}
