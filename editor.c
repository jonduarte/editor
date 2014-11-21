#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <curses.h>

#define PAD_LINE_NUM 3

static int running = 1;

static int max_rows,
           max_cols;

typedef struct line {
  char contents[80];
  int line_num;
  struct line *next;
} line_t;

void print_buffer(line_t *buffer) {
  if(buffer == NULL)
    return;

  attron(COLOR_PAIR(2));
  printw("%3d ", buffer->line_num);
  attroff(COLOR_PAIR(2));

  printw("%s", buffer->contents);

  print_buffer(buffer->next);
}

void close(int _val)
{

  running = 0;
}

void draw(int left, int top);

int main(int argc, char *argv[])
{

  signal(SIGINT, close);

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

  initscr();
  getmaxyx(stdscr, max_rows, max_cols);
  start_color();

  init_pair(1,COLOR_WHITE, COLOR_BLACK);
  init_pair(2,COLOR_CYAN, COLOR_BLACK);
  bkgd(COLOR_PAIR(1));
  move(0, 0);
  print_buffer(head);
  refresh();
  move(0, PAD_LINE_NUM);
  keypad(stdscr, TRUE);

  int row = 0,
      col = 0;

  cbreak();
  noecho();

  while(running) {
    int c = getch();

    switch(c) {
      case 'j':
        row++;
        break;
      case 'k':
        row--;
        break;
      case 'l':
        col++;
        break;
      case 'h':
        col--;
        break;
    }

    if(row > max_rows)
      row = max_rows;

    if(row < 0)
      row = 0;

    if(col > max_cols)
      col = max_cols;

    if(col < PAD_LINE_NUM)
      col = PAD_LINE_NUM;

    draw(row, col);
  }

  endwin();

  return 0;
}

void draw(int row, int col) {
  move(row, col);
  refresh();
}
