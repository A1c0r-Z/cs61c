#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);

/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.
  game_state_t* game = malloc(sizeof(game_state_t));
  if (game == NULL) {
    return NULL; // 返回空指针表示内存分配失败
  }
  game->num_rows = 18;
  game->board = (char **)malloc(18 * sizeof(char *));
  if (game->board == NULL) {
    free(game); // 释放之前分配的内存
    return NULL; // 返回空指针表示内存分配失败
  }
  for(int i=0;i<18;i++)
  {
    game->board[i] = (char *)malloc(21*sizeof(char));
    if(game->board[i] ==NULL)
    {
      for(int j=0;j<i;j++){
        free(game->board[j]);
      }
      free(game->board);
      free(game);
    }
    if(i!=0&&i!=17&&i!=2){
      strcpy(game->board[i],"#                  #");
    }
  }
  strcpy(game->board[0],"####################");
  strcpy(game->board[17],"####################");
  strcpy(game->board[2],"# d>D    *         #");


  game->num_snakes = 1;
  game->snakes = (snake_t *)malloc(game->num_snakes * sizeof(snake_t));
  game->snakes[0].tail_row = 2;
  game->snakes[0].tail_col = 2;
  game->snakes[0].head_row = 2;
  game->snakes[0].head_col = 4;
  game->snakes[0].live = true;
  return game;
}

/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.
  for(int i=0;i<state->num_rows;i++)
  {
    free(state->board[i]);
  }
  free(state->board);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  if (state == NULL || fp == NULL) {
    return;
  }

  for (int row = 0; row < state->num_rows; row++) {
    // for(int j=0;state->board[row][j]!='\0'&&state->board[row][j]!=EOF;j++){
      fprintf(fp, "%s", state->board[row]);
    
    fprintf(fp, "\n");
  }
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
  if(c=='w'||c=='a'||c=='s'||c=='d'){
    return true;
  }
  return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
  if(c=='W'||c=='A'||c=='S'||c=='D'||c=='x'){
    return true;
  }
  return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  if(c=='W'||c=='A'||c=='S'||c=='D'||
     c=='x'||
     c=='w'||c=='a'||c=='s'||c=='d'|| 
     c=='<'||c=='>'||c=='^'||c=='v')
  {
    return true;
  }
  return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  if(c=='<') return 'a';
  if(c=='>') return 'd';
  if(c=='v') return 's';
  if(c=='^') return 'w';
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  if(c=='W') return '^';
  if(c=='A') return '<';
  if(c=='S') return 'v';
  if(c=='D') return '>';
  return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implement this function.
  if(c=='v'||c=='s'||c=='S') return cur_row+1;
  if(c=='^'||c=='w'||c=='W') return cur_row-1;
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  // TODO: Implement this function.
  if(c=='>'||c=='d'||c=='D') return cur_col+1;
  if(c=='<'||c=='a'||c=='A') return cur_col-1;
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t snake= state->snakes[snum];
  char head_c = state->board[snake.head_row][snake.head_col];
  return state->board[get_next_row(snake.head_row,head_c)][get_next_col(snake.head_col,head_c)];
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t snake = state->snakes[snum];
  char head_c = state->board[snake.head_row][snake.head_col];
  state->board[get_next_row(snake.head_row,head_c)][get_next_col(snake.head_col,head_c)] = head_c;
  state->board[snake.head_row][snake.head_col] = head_to_body(head_c);
  state->snakes[snum].head_row = get_next_row(snake.head_row,head_c);
  state->snakes[snum].head_col = get_next_col(snake.head_col,head_c);
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t snake = state->snakes[snum];
  char tail_c = state->board[snake.tail_row][snake.tail_col];
  
  state->board[get_next_row(snake.tail_row,tail_c)][get_next_col(snake.tail_col,tail_c)] 
      = body_to_tail(state->board[get_next_row(snake.tail_row,tail_c)][get_next_col(snake.tail_col,tail_c)]);
  state->board[snake.tail_row][snake.tail_col] = ' ';
  state->snakes[snum].tail_row = get_next_row(snake.tail_row,tail_c);
  state->snakes[snum].tail_col = get_next_col(snake.tail_col,tail_c);
  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  char c;
  for(unsigned int i=0;i<state->num_snakes;i++)
  {
    if(state->snakes[i].live==false) continue;
    c = next_square(state, i);
    if(c=='#'||is_snake(c))
    {
      state->board[state->snakes[i].head_row][state->snakes[i].head_col] = 'x';
      state->snakes[i].live = false;
    }
    else if(c=='*')
    {
      update_head(state, i);
      add_food(state);
    }
    else
    {
      update_head(state, i);
      update_tail(state, i);
    }
  }
  return;
}

/* Task 5 */

game_state_t* load_board(FILE* file) {
  unsigned int num_rows = 0;
  unsigned row = 0;
  int c;
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      num_rows++;
    }
  } 
  fseek(file, 0, SEEK_SET);
  unsigned int* arr = (unsigned int*)calloc((num_rows),sizeof(unsigned int));
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      row++;
    }else{
      arr[row]++;
    }
  }
  game_state_t* game = (game_state_t*)malloc(sizeof(game_state_t));
  if (game == NULL) {
    return NULL; 
  }

  game->num_rows = num_rows;

  game->board = (char**)malloc(num_rows * sizeof(char*));
  if (game->board == NULL) {
    free(game);
    return NULL; 
  }

  fseek(file, 0, SEEK_SET);

  row = 0;
  unsigned int col = 0;
  for(int i=0;i<num_rows;i++)
  {
    game->board[i] = (char*)malloc((arr[i] + 1) * sizeof(char));
  }
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      game->board[row][col] = '\0'; 
      row++;
      col = 0;
    } else {
      game->board[row][col] = (char)c;
      col++;
    }
  }
  game->board[num_rows-1][arr[num_rows-1]] = '\0';
  
  // col = 0;
  // game->board[0] = (char*)malloc((max_col+1) * sizeof(char));
  // while ((c = fgetc(file)) != EOF) {
  //   if (c == '\n') {
  //     game->board[row] = (char*)malloc((max_col + 1) * sizeof(char));
  //     // if (game->board[row] == NULL) {
  //     //   free_state(game); 
  //     //   return NULL; 
  //     // }
  //     game->board[row][col] = '\0'; 
  //     row++;
  //     col = 0;
  //   } else {
  //     game->board[row][col] = c;
  //     col++;
  //   }
  // }
  free(arr);
  game->num_snakes = 0;
  game->snakes = NULL;

  return game;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  unsigned int row = state->snakes[snum].tail_row;
  unsigned int col = state->snakes[snum].tail_col;
  char c = state->board[row][col];
  while(!is_head(c))
  {
    row = get_next_row(row,c);
    col = get_next_col(col,c);
    c = state->board[row][col];
  }
  state->snakes[snum].head_row = row;
  state->snakes[snum].head_col = col;
  return;
}

// /* Task 6.2 */
// game_state_t* initialize_snakes(game_state_t* state) {
//   // TODO: Implement this function.
//   // for(int i=0;i<state->num_snakes;i++)
//   // {
//   //   for(int j=0;j<state->snakes[i].)
//   // }
//   return state;
// }

game_state_t* initialize_snakes(game_state_t* state) {

  unsigned row = state->num_rows;

  unsigned num_snakes = 0;

  for (unsigned int y = 0; y < row; y++) {
    for (unsigned x = 0; state->board[y][x+1]!='\0'; x++) {
      if(is_tail(state->board[y][x]))
      {
        num_snakes++;
      }
    }
  }
  state->snakes = (snake_t*)malloc(num_snakes*sizeof(snake_t));
  unsigned n = 0;
  for (unsigned int y = 0; y < row; y++) {
    for (unsigned x = 0; state->board[y][x+1]!='\0'; x++) {
      if(is_tail(state->board[y][x]))
      {
        state->snakes[n].live = true;
        state->snakes[n].tail_col = x;
        state->snakes[n].tail_row = y;
        find_head(state,n);
        if(state->board[state->snakes[n].head_row][state->snakes[n].head_col]=='x') 
          state->snakes[n].live = false;
        n++;
      }
    }
  }

  state->num_snakes = num_snakes;

  return state;
}