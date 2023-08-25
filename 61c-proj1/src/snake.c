#include <stdio.h>
#include <string.h>

#include "snake_utils.h"
#include "state.h"
// int main(int argc, char* argv[]) {
//   bool io_stdin = false;
//   char* in_filename = NULL;
//   char* out_filename = NULL;
//   game_state_t* state = NULL;

//   // Parse arguments
//   for (int i = 1; i < argc; i++) {
//     if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
//       if (io_stdin) {
//         fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
//         return 1;
//       }
//       in_filename = argv[i + 1];
//       i++;
//       continue;
//     } else if (strcmp(argv[i], "--stdin") == 0) {
//       if (in_filename != NULL) {
//         fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
//         return 1;
//       }
//       io_stdin = true;
//       continue;
//     }
//     if (strcmp(argv[i], "-o") == 0 && i < argc - 1) {
//       out_filename = argv[i + 1];
//       i++;
//       continue;
//     }
//     fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
//     return 1;
//   }

//   // Do not modify anything above this line.

//   /* Task 7 */

//   // Read board from file, or create default board
//   if (in_filename != NULL) {
//     // TODO: Load the board from in_filename
//     FILE* file = fopen(in_filename, "r");
//     if(file==NULL) return -1;
//     game_state_t* state = load_board(file);
//     fclose(file);
//     // TODO: If the file doesn't exist, return -1

//     // TODO: Then call initialize_snakes on the state you made
//     initialize_snakes(state);
//   } else if (io_stdin) {
//     // TODO: Load the board from stdin
//     game_state_t* state = load_board(stdin);
//     // TODO: Then call initialize_snakes on the state you made
//     initialize_snakes(state);
//   } else {
//     // TODO: Create default state
//     create_default_state();
//   }

//   // TODO: Update state. Use the deterministic_food function
//   // (already implemented in snake_utils.h) to add food.
//   deterministic_food(state);
//   // Write updated board to file or stdout
//     FILE* out_file = fopen(out_filename, "w");
//     if(out_file==NULL) return -1;
//     print_board(state,out_file);
//     fclose(out_file);
//     // TODO: Save the board to out_filename
//     // TODO: Print the board to stdout
//     print_board(state,stdout);

//   // TODO: Free the state
//   free_state(state);
//   return 0;
// }
int main(int argc, char* argv[]) {
  bool io_stdin = false;
  char* in_filename = NULL;
  char* out_filename = NULL;
  game_state_t* state = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
      if (io_stdin) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      in_filename = argv[i + 1];
      i++;
      continue;
    } else if (strcmp(argv[i], "--stdin") == 0) {
      if (in_filename != NULL) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      io_stdin = true;
      continue;
    }
    if (strcmp(argv[i], "-o") == 0 && i < argc - 1) {
      out_filename = argv[i + 1];
      i++;
      continue;
    }
    fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
    return 1;
  }

  // Do not modify anything above this line.

  /* Task 7 */

  // Read board from file, or create default board
  if (in_filename != NULL) {
    FILE* file = fopen(in_filename, "r");
    if (file == NULL) {
      return -1;
    }

    state = load_board(file);
    if(state==NULL) return -1;
    fclose(file);

    if (state == NULL) {

      return -1;
    }

    initialize_snakes(state);
  } else if (io_stdin) {
    state = load_board(stdin);
    if (state == NULL) {
      return -1;
    }

    initialize_snakes(state);
  } else {
    state = create_default_state();
    if (state == NULL) {

      return -1;
    }
  }


  update_state(state, deterministic_food);

  save_board(state, out_filename);
  print_board(state, stdout);


  free_state(state);

  return 0;
}