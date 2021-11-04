#include "maze.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

bool find_marker(char ch, char** maze, int height, int width, int& row, int& column){
  for (int r = 0; r < height; r++){
    for (int c = 0; c < width; c++){
      if (maze[r][c]== ch){
        row = r;
        column = c;
        return true;
      }
    }
  }
  row = -1;
  column = -1;
  return false;
}

bool valid_solution(const char* path, char** maze, int height, int width){
  if (*path == '\0'){ return false; }

  //initialise starting position
  int row = 0, column = 0;
  find_marker('>', maze, height, width, row, column);

  for (int i = 0; i < strlen(path); i++){
    char direction = path[i];
    if (direction == 'N'){ row -= 1; }
    else if (direction == 'S'){ row += 1; }
    else if (direction == 'W'){ column -= 1; }
    else if (direction == 'E'){ column += 1; }
    else { return false; }
    if (row > height-1 || row < 0 || column > width-1 || column < 0){ return false; }
    if (maze[row][column] == '+' ||maze[row][column] == '-' || maze[row][column] == '|'){ return false; }
  }
  return true;
}

bool valid_solution2(const char* path, char** maze, int height, int width, char start){
  if (*path == '\0'){ return false; }

  //initialise starting position
  int row = 0, column = 0;
  find_marker(start, maze, height, width, row, column);

  for (int i = 0; i < strlen(path); i++){
    char direction = path[i];
    if (direction == 'N'){ row -= 1; }
    else if (direction == 'S'){ row += 1; }
    else if (direction == 'W'){ column -= 1; }
    else if (direction == 'E'){ column += 1; }
    else { return false; }
    if (row > height-1 || row < 0 || column > width-1 || column < 0){ return false; }
    if (maze[row][column] == '+' ||maze[row][column] == '-' || maze[row][column] == '|'){ return false; }
  }
  return true;
}

void last_move(const char* path, char*move){
  int length = strlen(path);
  if(length > 0){
    *move = path[length-1]; 
  }
  *(move+1) = '\0';
}

void draw_solution(const char*path, char**maze, const int height, const int width, const char start){
  int row = 0, column = 0;
  find_marker(start, maze, height, width, row, column);
  maze[row][column] = '#';
  for (int i = 0; i < strlen(path); i++){
    if( path[i] == 'N'){ row--; maze[row][column] = '#'; }
    if( path[i] == 'S'){ row++; maze[row][column] = '#'; }
    if( path[i] == 'W'){ column--; maze[row][column] = '#'; }
    if( path[i] == 'E'){ column++; maze[row][column] = '#'; }
  }
  //resetlocation();
}

void resetlocation(){
  locations.clear();
  current_r = -1; 
  current_c = -1;
}

static char solution[2048];
const char* find_path(char** maze, const int height, const int width, const char start, const char end){

  int row = 0, column = 0;
  // if solved
  find_marker(end, maze, height, width, row, column);
  if (row == current_r && column == current_c){ 
    return solution; 
  }

  //if first time the function is called
  if (current_r == -1 && current_c == -1){
    find_marker(start, maze, height, width, row, column);
    strcpy(solution,""); 
    //remember where we are
    current_r = row;
    current_c = column;
    vector<int> v;
    v.push_back(row);
    v.push_back(column);
    locations.push_back(v);
    if(valid_solution2(find_path(maze,height,width,start,end), maze, height, width,start)){ 
      draw_solution(solution, maze,height, width,start); 
      //resetlocation();
      return solution;
    }
  }

  char tmp[512];
  char previous_move[2];//cannot go back and forth
  //recursively solving the problem
  last_move(solution, previous_move);
  if(strcmp("N",previous_move) != 0){
    if (current_r+1 < height && (maze[current_r+1][current_c] == ' ' || maze[current_r+1][current_c] == end)){
      //if not there before
      vector<int> v;
      v.push_back(current_r+1);
      v.push_back(current_c);
      if (find(locations.begin(), locations.end(), v) == locations.end()){ 
        //make copy of the current path before writing
        strcpy(tmp,solution);
        strcat(solution,"S");
        current_r++;
        locations.push_back(v);
        if (valid_solution2(find_path(maze,height,width,start,end), maze, height, width,start)){ return solution;}
        //not valid: undo this move
        current_r--;
        strcpy(solution,tmp);
        locations.pop_back();
      }
    }
  }

  if (strcmp("E",previous_move) != 0){
    if (current_c > 0 && (maze[current_r][current_c - 1] == ' ' || maze[current_r][current_c - 1] == end)){
      //if not there before
      vector<int> v;
      v.push_back(current_r);
      v.push_back(current_c-1);
      if (find(locations.begin(), locations.end(), v) == locations.end()){
         //make copy of the current path before writing
        strcpy(tmp, solution);
        strcat(solution, "W");
        current_c--;
        locations.push_back(v);
        if (valid_solution2(find_path(maze, height, width, start, end), maze, height, width,start)){ return solution; }
        //not valid: undo this move
        current_c++;
        strcpy(solution, tmp);
        locations.pop_back();
      }
    }
  } 

  if(strcmp("S",previous_move) != 0){
    if (current_r > 0 && (maze[current_r-1][current_c] == ' ' || maze[current_r-1][current_c] == end)){
      //if not there before
      vector<int> v;
      v.push_back(current_r-1);
      v.push_back(current_c);
      if (find(locations.begin(), locations.end(), v) == locations.end()){
        //make copy of the current path before writing
        strcpy(tmp,solution);
        strcat(solution,"N");
        current_r--;
        locations.push_back(v);
        if (valid_solution2(find_path(maze,height,width,start,end), maze, height, width,start)){ return solution;}
        //not valid: undo this move
        current_r++;
        strcpy(solution,tmp);
        locations.pop_back();
      }
    }
  }


  if(strcmp("W",previous_move) != 0){
    if (current_c+1 < width && (maze[current_r][current_c+1] == ' ' || maze[current_r][current_c+1] == end)){
      //if not there before 
      vector<int> v;
      v.push_back(current_r);
      v.push_back(current_c+1);
      if (find(locations.begin(), locations.end(), v) == locations.end()){
        //make copy of the current path before writing
        strcpy(tmp,solution);
        strcat(solution,"E");
        current_c++;
        locations.push_back(v);
        if (valid_solution2(find_path(maze,height,width,start,end), maze, height, width,start)){ return solution;}
        //not valid: undo this move
        current_c--;
        strcpy(solution,tmp);
        locations.pop_back();
      }
    }
  }

  //exhausted all possibilities
  return "No Solution";
}