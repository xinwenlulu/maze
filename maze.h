#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <string>


using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function prototypes to the end of this file. */

/* helper functions to allocate and deallocate dynamic 2D arrays */
char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **m, int rows);

/* pre-supplied function to load a maze from a file */
char **load_maze(const char *filename, int &height, int &width);

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width);

bool compare(const char* str1, const char* str2);
bool find_marker(char ch, char** maze, int height, int width, int& row, int& column);
bool valid_solution(const char* path, char** maze, int height, int width);
const char* find_path(char** maze, const int height, const int width, const char start, const char end);

void last_move(const char* path, char*move);
void draw_solution(const char*path, char**maze, const int height, const int width, const char start);
bool valid_solution2(const char* path, char** maze, int height, int width, char start);

static vector<vector<int> > locations;
static int current_r = -1, current_c = -1;
void resetlocation();