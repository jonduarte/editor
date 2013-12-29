#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR    81
#define MAX_FN_CHAR 10
#define MAX_CMDCHAR 5

typedef struct node{
  char content[MAX_CHAR];
  int val;
  void *next;
} node;

typedef struct list{
  node *HEAD;
  node *TAIL;
  int line_num;
} list;

void read(list *buffer, char *line)
{
  node *next;
  next = (node *)malloc(sizeof(node));
  strcpy(next->content, line);
  next->next = NULL;

  if(buffer->HEAD == NULL){
    buffer->HEAD = next;
    buffer->TAIL = next;
  }else{
    buffer->TAIL->next = next;
    buffer->TAIL = buffer->TAIL->next;
  }
}

void add_int(list * buffer, int line_num, int val)
{
  int counter = 1;
  node *next;

  next = (node *)malloc(sizeof(node));
  next->val = val;
  next->next = NULL;

  if (line_num == 0 && buffer->HEAD == NULL && buffer->TAIL == NULL){
    next->next = buffer->HEAD;
    buffer->HEAD = next;
    buffer->TAIL = next;
  }else if(line_num == 0){
    next->next = buffer->HEAD;
    buffer->HEAD = next;
  }else{
    for(node *i = buffer->HEAD; i != NULL; i = i->next){
      if(counter == line_num){
        next->next = i->next;
        i->next = next;
      }
      counter ++;
    }

    if(line_num == buffer->line_num){
      buffer->TAIL = next;
    }
  }
  buffer->line_num = buffer->line_num + 1;
}

void clean(list *buffer)
{
  node *next;
  node *i;

  for(i = buffer->HEAD; i != NULL; i = i->next){
    next = buffer->HEAD->next;
    free(buffer->HEAD);
    buffer->HEAD = next;
  }

  buffer->HEAD = NULL;
  buffer->TAIL = NULL;
  buffer->line_num = 0;
}

void print(list *buffer, char *filename)
{
  node *i;
  int count = 0;

  printf("%s\n", filename);

  for(i = buffer->HEAD; i != NULL; i = i->next){
    if(count < 9){
      count ++;
      printf("0%i %s", count, i->content);
    }else{
      count ++;
      printf("%i %s", count, i->content);
    }
  }
}

void add(list *buffer, int line_num, char *line)
{
  int counter = 1;

  node *next;
  next = (node *)malloc(sizeof(node));
  strcpy(next->content, line);
  next->next = NULL;

  if(line_num == 0 && buffer->HEAD == NULL && buffer->TAIL == NULL){
    next->next = buffer->HEAD;
    buffer->HEAD = next;
    buffer->TAIL = next;
  }else if(line_num == 0){
    next->next = buffer->HEAD;
    buffer->HEAD = next;
  }else{
    for(node *i = buffer->HEAD; i != NULL; i = i->next){
      if(counter == line_num){
        next->next = i->next;
        i->next = next;
      }
      counter ++;
    }

    if(line_num == buffer->line_num){
      buffer->TAIL = next;
    }
  }
  buffer->line_num = buffer->line_num + 1;
}

void delete(list *buffer, int at_line)
{
  node *i;
  node *prev_node;
  node *follow_node;
  int counter = 1;

  if (at_line == 0){
    clean(buffer);
  }else{
    if(at_line == 1){
      free(buffer->HEAD);
      buffer->HEAD = buffer->HEAD->next;
    }else if(at_line == buffer->line_num){
      for(i = buffer->HEAD; i != NULL; i = i->next){
        if(counter == buffer->line_num - 1){
          free(i->next);
          i->next = NULL;
          buffer->TAIL = i;
        }
        counter ++;
      }
    }
    else{
      prev_node = buffer->HEAD;
      for(i = buffer->HEAD->next; i != NULL; i = i->next){
        follow_node = i->next;
        if(counter == at_line - 1){
          free(prev_node->next);
          prev_node->next = follow_node;
        }
        counter ++;
        prev_node = i;
      }
    }
    buffer->line_num --;
  }
}

void replace(list *buffer, int line_number, char *line)
{
  node *i;
  int counter = 1;

  for(i = buffer->HEAD; i != NULL; i = i->next){
    if(counter == line_number){
      strcpy(i->content, line);
    }
    counter++;
  }
}

void save(list *buffer, char *filename)
{
  FILE *handler;

  handler = fopen(filename, "w");

  if(handler == NULL){
    printf("Can't open file for writing\n");
    fclose(handler);
  }else{
    for(node *i = buffer->HEAD; i != NULL; i = i->next){
      fprintf(handler, "%s", i->content);
    }
    fclose(handler);
  }
}

void block_in_last(list *buffer, list *block)
{
  if(buffer->HEAD == NULL && buffer->TAIL == NULL){
    buffer->HEAD = block->HEAD;
    buffer->TAIL = block->TAIL;
  }else{
    buffer->TAIL->next = block->HEAD;
    buffer->TAIL = block->TAIL;
  }
}

void block_insert(list *buffer, list *block, int at_line)
{
  node *i;
  int counter;
  counter = 1;

  if(at_line == 0){
    if(buffer->HEAD != NULL && buffer->TAIL != NULL){
      block->TAIL->next = block->HEAD;
      block->HEAD = block->HEAD;
    }else{
      block_in_last(buffer, block);
    }
  }else{
    for(i = buffer->HEAD; i != NULL; i  = i->next){
      if(counter == at_line){
        block->TAIL->next = i->next;
        i->next = block->HEAD;
      }
      counter++;
    }
  }
}

void pop_str(list *buffer, char *tmp)
{
  int line_num = 1;

  for(node *i = buffer->HEAD; i != NULL; i = i->next){
    if(line_num == buffer->line_num){
      strcpy(tmp, i->content);
    }
    line_num++;
  }
  delete(buffer, buffer->line_num);
}

void undo_add(char *tmp_cmd, list *buffer, int at_line)
{
  if(strlen(tmp_cmd) == 1){
    delete(buffer, buffer->line_num);
  }else{
    delete(buffer, at_line + 1);
  }
}

void undo_replace(list *buffer, list *hist_content, int at_line)
{
  char tmp_str[MAX_CHAR];

  pop_str(hist_content, tmp_str);
  replace(buffer, at_line, tmp_str);
}

void undo_delete(char *tmp_cmd, list *buffer, list *hist_content, list *back, int at_line)
{

  char tmp_str[MAX_CHAR];
  int cntr = 1;
  int tmp_int;

  if(strcmp(tmp_cmd, "d0\n") == 0){

    for(node *j = hist_content->HEAD; j != NULL; j = j->next){
      if(cntr == hist_content->line_num){
        tmp_int = j->val;
      }
      cntr ++;
    }

    delete(hist_content, hist_content->line_num);
    cntr = 1;

    for(int k = 1; k <= tmp_int; k++){
      for(node *h = back->HEAD; h != NULL; h = h->next){
        if(cntr == back->line_num){
          strcpy(tmp_str, h->content);
        }
        cntr++;
      }
      delete(back, back->line_num);
      cntr = 1;
      add(buffer, 0, tmp_str);
    }
  }else{
    pop_str(hist_content, tmp_str);
    add(buffer, at_line - 1, tmp_str);
  }
}

void undo_block_insert(char *tmp_cmd, list *buffer, list *hist_content, int at_line)
{
  int cntr = 1;
  int tmp_int;

  for(node *j = hist_content->HEAD; j != NULL; j = j->next){
    if(cntr == hist_content->line_num){
      tmp_int = j->val;
    }
    cntr++;
  }

  delete(hist_content, hist_content->line_num);

  if(strcmp(tmp_cmd, "m\n") == 0){
    for(int k = 1; k <= tmp_int; k++){
      delete(buffer, buffer->line_num);
    }
  }else{
    for(int k = 1; k <= tmp_int; k++){
      delete(buffer, at_line + 1);
    }
  }

}

void undo(list *buffer, list *history, list *hist_content, list *back)
{
  int at_line;
  char cmd;

  char tmp_cmd[MAX_CMDCHAR];

  pop_str(history, tmp_cmd);
  cmd = tmp_cmd[0];
  at_line = atoi(&tmp_cmd[1]);

  switch(cmd){
    case 'a':
      undo_add(tmp_cmd, buffer, at_line);
      break;

    case 'r':
      undo_replace(buffer, hist_content, at_line);
      break;

    case 'd':
      undo_delete(tmp_cmd, buffer, hist_content, back, at_line);
      break;

    case 'm':
      undo_block_insert(tmp_cmd, buffer, hist_content, at_line);
      break;

    default: break;
  }
}

void do_replace(list *buffer, list *history, list *hist_content, char *param, char *rest_string, int at_line)
{
  add(history, history->line_num, param);
  int cntr = 1;
  for(node *i = buffer->HEAD; i != NULL; i = i->next){
    if(at_line == cntr){
      add(hist_content, hist_content->line_num, i->content);
    }
    cntr++;
  }

  replace(buffer, at_line, rest_string);
}

void do_add_block(char *buffer_copy, list *buffer, list *history, list *hist_content, list *block, char *param, int at_line)
{
  char block_string[MAX_CHAR];

  add(history, history->line_num, param);
  while(1){
    fgets(block_string, MAX_CHAR, stdin);
    if(strcmp(block_string, "\0xb\n") == 0){
      break;
    }else{
      read(block, block_string);
      block->line_num ++;
      buffer->line_num = buffer->line_num + 1;
    }
  }
  if(strcmp(buffer_copy, "m\n") == 0){
    block_in_last(buffer, block);
  }else{
    block_insert(buffer, block, at_line);
  }

  add_int(hist_content, hist_content->line_num, block->line_num);
  block->HEAD = NULL;
  block->TAIL = NULL;
  block->line_num = 0;
}

void do_delete(list *buffer, list *history, list *hist_content, list *back, char *param, int at_line)
{
  if(at_line == 0){
    add(history, history->line_num, param);
    add_int(hist_content, hist_content->line_num, buffer->line_num);

    for(node *i  = buffer->HEAD; i != NULL; i = i->next){
      add(back, back->line_num, i->content);
    }

  }else{
    add(history, history->line_num, param);
    int cntr = 1;
    for(node *i = buffer->HEAD; i != NULL; i = i->next){
      if(at_line == cntr){
        add(hist_content, hist_content->line_num, i->content);
      }
      cntr ++;
    }
  }
  delete(buffer, at_line);
}

void do_add(list *buffer, list *history, char *param, char *rest_string, int at_line)
{
  add(history, history->line_num, param);
  if (strlen(param) == 1){
    add(buffer, buffer->line_num, rest_string);
  }else{
    add(buffer, at_line, rest_string);
  }
}

void do_print(char *buffer_copy, list *buffer, char *filename)
{
  if(strcmp(buffer_copy, "p\n") == 0){
    print(buffer, filename);
  }
}

void do_save(char *buffer_copy, list *buffer, list *history, list *hist_content, list *back, list *block, char *filename)
{
  if(strcmp(buffer_copy, "s\n") == 0){
    save(buffer, filename);
    clean(history);
    clean(hist_content);
    clean(back);
    clean(block);
  }
}

void do_quit(char *buffer_copy, list *buffer, char *filename)
{
  if(strcmp(buffer_copy, "q\n") == 0){
    save(buffer, filename);
    exit(0);
  }else if(strcmp(buffer_copy, "qns\n") == 0){
    exit(0);
  }
}

void do_undo(list *buffer, list *history, list *hist_content, list *back)
{
  if(history->line_num == 0){
    printf("Cannot undo\n");
  }else{
    undo(buffer, history, hist_content, back);
  }
}

int main(int argc, char *argv[]){
  int at_line;

  char filename[MAX_FN_CHAR];
  char strTmp[MAX_CHAR];
  char stringBuffer[MAX_CHAR];
  char buffer_copy[MAX_CHAR];
  char *param;
  char *rest_string;
  char cmd;

  list buffer;
  list block;
  list history;
  list hist_content;
  list back;

  clean(&buffer);
  clean(&block);
  clean(&history);
  clean(&hist_content);
  clean(&back);

  if (argc == 3){
    if(strcmp(argv[1], "-c") == 0){
      FILE *checker;
      checker = fopen(argv[2], "r");
      if (checker == NULL){
        fclose(checker);
        strcpy(filename, argv[2]);
        FILE *handler;
        handler = fopen(argv[2], "w");
        fclose(handler);
      }else{
        fclose(checker);
        printf("The specified file already exists\n");
        exit(1);
      }
    }else{
      printf("Usage: ./%s [-c] <FILE>\n", argv[0]);
      exit(1);
    }
  }else if(argc == 2){
    strcpy(filename, argv[1]);

    FILE *handler;
    handler = fopen(argv[1], "r");

    if ((handler = fopen(argv[1], "r")) == NULL){
      fclose(handler);
      printf("The specified file does not exist\n");
      exit(1);
    }else{
      while(fgets(strTmp, MAX_CHAR, handler)){
        read(&buffer, strTmp);
        buffer.line_num = buffer.line_num + 1;
      }
      fclose(handler);
    }
  }else{
    printf("Usage: ./%s [-c] <FILE>\n", argv[0]);
    exit(1);
  }

  while(1){
    printf(":");
    fgets(stringBuffer, MAX_CHAR, stdin);
    strcpy(buffer_copy, stringBuffer);

    param = strtok(stringBuffer, " ");
    do {
      rest_string = strtok(NULL, "");
    }while(strtok(NULL, " ") != NULL);
    at_line = atoi(&param[1]);
    cmd = param[0];

    switch(cmd){
      case 'a':
        do_add(&buffer, &history, param, rest_string, at_line);
        break;

      case 'd':
        do_delete(&buffer, &history, &hist_content, &back, param, at_line);
        break;

      case 'r':
        do_replace(&buffer, &history, &hist_content, param, rest_string, at_line);
        break;
      case 'm':
        do_add_block(buffer_copy, &buffer, &history, &hist_content, &block, param, at_line);
        break;

      case 'p':
        do_print(buffer_copy, &buffer, filename);
        break;

      case 's':
        do_save(buffer_copy, &buffer, &history, &hist_content, &back, &block, filename);
        break;

      case 'q':
        do_quit(buffer_copy, &buffer, filename);
        break;

      case 'u':
        do_undo(&buffer, &history, &hist_content, &back);
        break;

      default:
        break;
    }
  }
}
