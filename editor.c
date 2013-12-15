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
  int lineNum;
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

void addInt(list *fileBuffer, int atLine, int myVal){
  node *i;
  int counter;
  counter = 1;

  /* add a new node to our list */
  node *newNode;
  newNode = (node *)malloc(sizeof(node));
  newNode->val = myVal;
  newNode->next = NULL;

  if (atLine == 0 && fileBuffer->HEAD == NULL && fileBuffer->TAIL == NULL){
    newNode->next = fileBuffer->HEAD;
    fileBuffer->HEAD = newNode;
    fileBuffer->TAIL = newNode;
  }else if(atLine == 0){
    newNode->next = fileBuffer->HEAD;
    fileBuffer->HEAD = newNode;
  }else{
    for(i = fileBuffer->HEAD; i != NULL; i = i->next){
      if(counter == atLine){
        /* add */
        newNode->next = i->next;
        i->next = newNode;
      }
      counter ++;
    }

    if(atLine == fileBuffer->lineNum){
      fileBuffer->TAIL = newNode;
    }
  }
  fileBuffer->lineNum = fileBuffer->lineNum + 1;
}

void freeList(list *yourList){
  node *tmpNode;
  node *i;

  for(i = yourList->HEAD; i != NULL; i = i->next){
    tmpNode = yourList->HEAD->next;
    free(yourList->HEAD);
    yourList->HEAD = tmpNode;
  }

  yourList->HEAD = NULL;
  yourList->TAIL = NULL;
  yourList->lineNum = 0;
}

void print(list *fileBuffer, char *fileName){
  printf("%s\n", fileName);
  node *i;
  int count;
  count = 0;

  for(i = fileBuffer->HEAD; i != NULL; i = i->next){
    if(count < 9){
      count ++;
      printf("0%i %s", count, i->content);
    }else{
      count ++;
      printf("%i %s", count, i->content);
    }
  }
}

void add(list *fileBuffer, int atLine, char *restOfString){
  node *i;
  int counter;
  counter = 1;

  node *newNode;
  newNode = (node *)malloc(sizeof(node));
  strcpy(newNode->content, restOfString);
  newNode->next = NULL;

  if(atLine == 0 && fileBuffer->HEAD == NULL && fileBuffer->TAIL == NULL){
    newNode->next = fileBuffer->HEAD;
    fileBuffer->HEAD = newNode;
    fileBuffer->TAIL = newNode;
  }else if(atLine == 0){
    newNode->next = fileBuffer->HEAD;
    fileBuffer->HEAD = newNode;
  }else{
    for(i = fileBuffer->HEAD; i != NULL; i = i->next){
      if(counter == atLine){
        newNode->next = i->next;
        i->next = newNode;
      }
      counter ++;
    }
    if(atLine == fileBuffer->lineNum){
      fileBuffer->TAIL = newNode;
    }
  }
  fileBuffer->lineNum = fileBuffer->lineNum + 1;
}

void delete(list *fileBuffer, int atLine){
  node *i;
  node *prevNode;
  node *followNode;
  int counter;
  counter = 1;

  if (atLine == 0){
    freeList(fileBuffer);
  }else{
    if(atLine == 1){
      free(fileBuffer->HEAD);
      fileBuffer->HEAD = fileBuffer->HEAD->next;
    }else if(atLine == fileBuffer->lineNum){
      for(i = fileBuffer->HEAD; i != NULL; i = i->next){
        if(counter == fileBuffer->lineNum - 1){
          free(i->next);
          i->next = NULL;
          fileBuffer->TAIL = i;
        }
        counter ++;
      }
    }
    else{
      prevNode = fileBuffer->HEAD;
      for(i = fileBuffer->HEAD->next; i != NULL; i = i->next){
        followNode = i->next;
        if(counter == atLine - 1){
          free(prevNode->next);
          prevNode->next = followNode;
        }
        counter ++;
        prevNode = i;
      }
    }
    fileBuffer->lineNum --;
  }
}

void replace(list *fileBuffer, int atLine, char *restOfString){
  node *i;
  int counter;
  counter = 1;

  for(i = fileBuffer->HEAD; i != NULL; i = i->next){
    if(counter == atLine){
      strcpy(i->content, restOfString);
    }
    counter ++;
  }
}

void save(list *fileBuffer, char *fileName){
  FILE *handler;
  handler = fopen(fileName, "w");
  if(handler == NULL){
    printf("Can't open file for writing\n");
    fclose(handler);
  }else{
    node *i;
    for(i = fileBuffer->HEAD; i != NULL; i = i->next){
      fprintf(handler, "%s", i->content);
    }
    fclose(handler);
  }
}

void blockInLast(list *fileBuffer, list *blockBuffer){
  if(fileBuffer->HEAD == NULL && fileBuffer->TAIL == NULL){
    fileBuffer->HEAD = blockBuffer->HEAD;
    fileBuffer->TAIL = blockBuffer->TAIL;
  }else{
    fileBuffer->TAIL->next = blockBuffer->HEAD;
    fileBuffer->TAIL = blockBuffer->TAIL;
  }
}

void blockInsert(list *fileBuffer, list *blockBuffer, int atLine){
  node *i;
  int counter;
  counter = 1;

  if(atLine == 0){
    if(fileBuffer->HEAD != NULL && fileBuffer->TAIL != NULL){
      blockBuffer->TAIL->next = blockBuffer->HEAD;
      blockBuffer->HEAD = blockBuffer->HEAD;
    }else{
      blockInLast(fileBuffer, blockBuffer);
    }
  }else{
    for(i = fileBuffer->HEAD; i != NULL; i  = i->next){
      if(counter == atLine){
        blockBuffer->TAIL->next = i->next;
        i->next = blockBuffer->HEAD;
      }
      counter++;
    }
  }
}

void popStr(list *myList, char *tmp){
  node *i;
  int cntr;
  cntr = 1;
  for(i = myList->HEAD; i != NULL; i = i->next){
    if(cntr == myList->lineNum){
      strcpy(tmp, i->content);
    }
    cntr++;
  }
  delete(myList, myList->lineNum);
}

void undo(list *fileBuffer, list *cmdHistory, list *histContent, list *backBuffer){
  int tmpInt;
  int atLine;
  char cmd;

  char tmpCmd[MAX_CMDCHAR];
  char tmpStr[MAX_CHAR];

  node *j;
  node *h;
  int k;

  int cntr;
  cntr = 1;

  popStr(cmdHistory, tmpCmd);
  cmd = tmpCmd[0];
  atLine = atoi(&tmpCmd[1]);

  switch(cmd){
    case 'a': if(strlen(tmpCmd) == 1){
                delete(fileBuffer, fileBuffer->lineNum);
              }else{
                delete(fileBuffer, atLine + 1);
              }
              break;

    case 'r': popStr(histContent, tmpStr);
              replace(fileBuffer, atLine, tmpStr);
              break;

    case 'd': if(strcmp(tmpCmd, "d0\n") == 0){
                for(j = histContent->HEAD; j != NULL; j = j->next){
                  if(cntr == histContent->lineNum){
                    tmpInt = j->val;
                  }
                  cntr ++;
                }
                delete(histContent, histContent->lineNum);
                cntr = 1;

                for(k = 1; k <= tmpInt; k++){
                  for(h = backBuffer->HEAD; h != NULL; h = h->next){
                    if(cntr == backBuffer->lineNum){
                      strcpy(tmpStr, h->content);
                    }
                    cntr ++;
                  }
                  delete(backBuffer, backBuffer->lineNum);
                  cntr = 1;
                  add(fileBuffer, 0, tmpStr);
                }
              }else{
                popStr(histContent, tmpStr);
                add(fileBuffer, atLine - 1, tmpStr);
              }
              break;

    case 'm': for(j = histContent->HEAD; j != NULL; j = j->next){
                if(cntr == histContent->lineNum){
                  tmpInt = j->val;
                }
                cntr ++;
              }
              delete(histContent, histContent->lineNum);
            if(strcmp(tmpCmd, "m\n") == 0){
              for(k = 1; k <= tmpInt; k++){
                delete(fileBuffer, fileBuffer->lineNum);
              }
            }else{
              for(k = 1; k <= tmpInt; k++){
                delete(fileBuffer, atLine + 1);
              }
            }
            break;
    default: break;
  }
}
    
int main(int argc, char *argv[]){
  int atLine;
  char fileName[MAX_FN_CHAR];
  char strTmp[MAX_CHAR];
  char stringBuffer[MAX_CHAR];
  char bufferCopy[MAX_CHAR];
  char *param;
  char *restOfString;
  char cmd;
  char blockString[MAX_CHAR];

  list fileBuffer;
  fileBuffer.HEAD = NULL;
  fileBuffer.TAIL = NULL;
  fileBuffer.lineNum = 0;

  list blockBuffer;
  blockBuffer.HEAD = NULL;
  blockBuffer.TAIL = NULL;
  blockBuffer.lineNum = 0;

  // Undo list
  list cmdHistory;
  cmdHistory.HEAD = NULL;
  cmdHistory.TAIL = NULL;
  cmdHistory.lineNum = 0;

  list histContent;
  histContent.HEAD = NULL;
  histContent.TAIL = NULL;
  histContent.lineNum = 0;

  list backBuffer;
  backBuffer.HEAD = NULL;
  backBuffer.TAIL = NULL;
  backBuffer.lineNum = 0;

  if (argc == 3){
    if(strcmp(argv[1], "-c") == 0){
      FILE *checker;
      checker = fopen(argv[2], "r");
      if (checker == NULL){
        fclose(checker);
        strcpy(fileName, argv[2]);
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
    strcpy(fileName, argv[1]);

    FILE *handler;
    handler = fopen(argv[1], "r");

    if ((handler = fopen(argv[1], "r")) == NULL){
      fclose(handler);
      printf("The specified file does not exist\n");
      exit(1);
    }else{
      while(fgets(strTmp, MAX_CHAR, handler)){
        read(&fileBuffer, strTmp);
        fileBuffer.lineNum = fileBuffer.lineNum + 1;
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
    strcpy(bufferCopy, stringBuffer);

    param = strtok(stringBuffer, " ");
    do {
      restOfString = strtok(NULL, "");
    }while(strtok(NULL, " ") != NULL);
    atLine = atoi(&param[1]);
    cmd = param[0];

    switch(cmd){
      case 'a': add(&cmdHistory, cmdHistory.lineNum, param);
                if (strlen(param) == 1){
                  add(&fileBuffer, fileBuffer.lineNum, restOfString);
                }else{
                  add(&fileBuffer, atLine, restOfString);
                }
                break;

      case 'd': if(atLine == 0){
                  add(&cmdHistory, cmdHistory.lineNum, param);
                  addInt(&histContent, histContent.lineNum, fileBuffer.lineNum);
                  node *i;

                  for(i  = fileBuffer.HEAD; i != NULL; i = i->next){
                    add(&backBuffer, backBuffer.lineNum, i->content);
                  }
                }else{
                  add(&cmdHistory, cmdHistory.lineNum, param);
                  node *i;
                  int cntr;
                  cntr = 1;
                  for(i = fileBuffer.HEAD; i != NULL; i = i->next){
                    if(atLine == cntr){
                      add(&histContent, histContent.lineNum, i->content);
                    }
                    cntr ++;
                  }
                }
                delete(&fileBuffer, atLine);
                break;
    case 'r': add(&cmdHistory, cmdHistory.lineNum, param);
              node *i;
              int cntr;
              cntr = 1;
              for(i = fileBuffer.HEAD; i != NULL; i = i->next){
                if(atLine == cntr){
                  add(&histContent, histContent.lineNum, i->content);
                }
                cntr ++;
              }

              replace(&fileBuffer, atLine, restOfString);
              break;
    case 'm': add(&cmdHistory, cmdHistory.lineNum, param);
              while(1){
                fgets(blockString, MAX_CHAR, stdin);
                if(strcmp(blockString, "\0xb\n") == 0){
                  break;
                }else{
                  read(&blockBuffer, blockString);
                  blockBuffer.lineNum ++;
                  fileBuffer.lineNum = fileBuffer.lineNum + 1;
                }
              }
              if(strcmp(bufferCopy, "m\n") == 0){
                blockInLast(&fileBuffer, &blockBuffer);
              }else{
                blockInsert(&fileBuffer, &blockBuffer, atLine);
              }

              addInt(&histContent, histContent.lineNum, blockBuffer.lineNum);
              blockBuffer.HEAD = NULL;
              blockBuffer.TAIL = NULL;
              blockBuffer.lineNum = 0;
              break;

    case 'p': if(strcmp(bufferCopy, "p\n") == 0){
                print(&fileBuffer, fileName);
              }
              break;
    case 's': if(strcmp(bufferCopy, "s\n") == 0){
                save(&fileBuffer, fileName);
                freeList(&cmdHistory);
                freeList(&histContent);
                freeList(&backBuffer);
                freeList(&blockBuffer);
              }
              break;

    case 'q': if(strcmp(bufferCopy, "q\n") == 0){
                save(&fileBuffer, fileName);
                exit(0);
              }else
                if(strcmp(bufferCopy, "qns\n") == 0){
                  exit(0);
              }else{
                break;
              }

    case 'u': if(cmdHistory.lineNum == 0){
                printf("Cannot undo\n");
              }else{
                undo(&fileBuffer, &cmdHistory, &histContent, &backBuffer);
              }
              break;
    default:  break;
    }
  }
}





