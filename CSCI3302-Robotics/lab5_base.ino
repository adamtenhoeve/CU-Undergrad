/**
 * IMPORTANT: Read through the code before beginning implementation!
 * Your solution should fill in the various "TODO" items within this starter code.
 */

#include <sparki.h>
#define TRUE 1
#define FALSE 0

#define CYCLE_TIME .100

// Number of vertices to discretize the map
#define NUM_X_CELLS 4
#define NUM_Y_CELLS 4

// Map is ~60cm x 42cm
#define MAP_SIZE_X 0.6
#define MAP_SIZE_Y 0.42

#define BIG_NUMBER 9999


// Grid map from Lab 4: values of 1 indicate free space, 0 indicates empty space
bool world_map[NUM_Y_CELLS][NUM_X_CELLS];

// Destination (I,J) grid coordinates
int goal_i = 3;
int goal_j = 3;

// Start (I,J) grid coordinates
int source_i = 0;
int source_j = 0;

// Pointer to ordered sequence of waypoints
int *path = NULL;


void setup() {   
  // Dijkstra Setup -- initialize empty world map
  for (int j = 0; j < NUM_Y_CELLS; ++j) {
    for (int i = 0; i < NUM_X_CELLS; ++i) {
      world_map[j][i] = 1;
    }
  }

  //TODO: Set up your map here by setting individual cells to 0 to indicate obstacles
  world_map[0][1] = 0; // Example of setup code to indicate an obstacle at grid position (1,0)
  world_map[1][2] = 0; // Example of setup code to indicate an obstacle at grid position (1,1) This is a bool.
  world_map[2][3] = 0;
  
//  while(true){
//    sparki.clearLCD();
//    for (int j = 0; j < NUM_Y_CELLS; ++j) {
//      for (int i = 0; i < NUM_X_CELLS; ++i) {
//        sparki.print(world_map[j][i]);
//        sparki.print(' ');
//      }
//    }
//    sparki.updateLCD();
//  }
}

/*****************************
 * Dijkstra Helper Functions *
 *****************************/

// Return 1 if there are entries in range [0,inf) in arr
// otherwise return 0, signifying empty queue
bool is_empty(int *arr, int len) {
  for (int i=0; i < len; ++i) {
    if (arr[i] >= 0) {
      return FALSE;
    }
  }
  return TRUE;
}

// Return the index with the minimum value in int array "arr" of length "len"
// Assumes positive values only, with values of "-1" indicating 'empty'
int get_min_index(int *arr, int len) {
  int min_val=-1, min_idx=-1;
  for (int i=0;i < len; ++i) {
    if ((arr[i] < min_val && arr[i] != -1) || min_val == -1) {
      min_val = arr[i];
      min_idx = i;
    }
  }
//  while(true)
//  {
//   sparki.clearLCD();
//   sparki.print(min_idx);
//   sparki.updateLCD();
//   delay(1000);
//  }
  return min_idx;
}


/**********************************
 * Coordinate Transform Functions *
 **********************************/

// Converts a vertex index into (I,J) coordinates
// Returns 0 if something went wrong -- assume invalid i and j values being set
bool vertex_index_to_ij_coordinates(int v_idx, int *i, int *j) {
  *i = v_idx % NUM_X_CELLS;
  *j = v_idx / NUM_X_CELLS;
  
  if (*i < 0 || *j < 0 || *i >= NUM_X_CELLS || *j >= NUM_Y_CELLS) return FALSE;
  return TRUE;
}

// Converts (I,J) coordinates into a vertex index
int ij_coordinates_to_vertex_index(int i, int j) {
  return j*NUM_X_CELLS + i;  
}

// Convert (i,j) coordinates into world coordinates
// Returns 0 if something went wrong -- assume invalid x and y values are being set
// Returns 1 otherwise. Assigned x and y values are the middle of cell (i,j)
bool ij_coordinates_to_xy_coordinates(int i, int j, float *x, float *y) {
  if (i < 0 || j < 0 || i >= NUM_X_CELLS || j >= NUM_Y_CELLS) return FALSE;
  
  *x = (i+0.5)*(MAP_SIZE_X/NUM_X_CELLS);
  *y = (j+0.5)*(MAP_SIZE_Y/NUM_Y_CELLS);
  return TRUE;  
}

// Convert (x,y) world coordinates into (i,j) grid coordinates
// Returns 0 if something went wrong -- assume invalid x and y values are being set
// Returns 1 otherwise. x and y values are the middle of cell (i,j)
bool xy_coordinates_to_ij_coordinates(float x, float y, int *i, int *j) {
  if (x < 0 || y < 0 || x >= MAP_SIZE_X || y >= MAP_SIZE_Y) return FALSE;
  
  *i = int((x/MAP_SIZE_X) * NUM_X_CELLS);
  *j = int((y/MAP_SIZE_Y) * NUM_Y_CELLS);

  return TRUE;  
}

/**********************************
 *      Core Dijkstra Functions   *
 **********************************/

// Returns the cost of moving from vertex_source to vertex_dest
int get_travel_cost(int vertex_source, int vertex_dest) {
  /* INSTRUCTIONS: 
      This function should return 1 if:
        vertex_source and vertex_dest are neighbors in a 4-connected grid (i.e., N,E,S,W of each other but not diagonal) and neither is occupied in world_map (i.e., world_map isn't 0 for either)
   
      This function shouuld return BIG_NUMBER if:
        vertex_source corresponds to (i,j) coordinates outside the map
        vertex_dest corresponds to (i,j) coordinates outside the map
        vertex_source and vertex_dest are not adjacent to each other (i.e., more than 1 move away from each other)
  */

  int i1, j1, i2, j2;
  vertex_index_to_ij_coordinates(vertex_source, &i1, &j1);
  vertex_index_to_ij_coordinates(vertex_dest, &i2, &j2);
  if (!world_map[j2][i2] || !world_map[j1][i1]) return BIG_NUMBER; // BHEdit 
  if ((abs(i1-i2) == 1 && (j1-j2 == 0)) || ((i1-i2 == 0) && (abs(j1-j2) == 1)))
  {
    return 1;
  }
  return BIG_NUMBER;
}


// Allocate and return a list of ints corresponding to the "prev" variable in Dijkstra's algorithm
// The returned array prev can be treated as a lookup table:  prev[vertex_index] = next vertex index on the path back to source_vertex
int *run_dijkstra(int source_vertex) {
  // Array mapping vertex_index to distance of shortest path from source_vertex to vertex_index.
  int dist[NUM_X_CELLS*NUM_Y_CELLS];
  
  // Queue for identifying which vertices are still being explored -- Q_cost[vertex_index] = shortest known dist to get to vertex_index. 
  // Q_cost[vertex_index] = -1 if the vertex is no longer being considered.
  int Q_cost[NUM_Y_CELLS*NUM_X_CELLS]; 

  // Initialize memory for prev array
  int *prev = new int[NUM_X_CELLS*NUM_Y_CELLS];

  /**
   * TODO: Insert your Dijkstra's code here
   */
   int i;
   for (i = 0; i < 16; i++) {
    dist[i] = BIG_NUMBER;
    prev[i] = -1;
    Q_cost[i] = -1;
  }

  dist[source_vertex] = 0;
  Q_cost[source_vertex] = 0;
  int iterator = 0;
  while (!is_empty(Q_cost, 16)) {
    int m = get_min_index(Q_cost, 16); // m is the index of of the cell which will find adjacent cells
//    while(true){
//      sparki.clearLCD();
////      for(int i = 0; i < 16; i++){
////        sparki.print(Q_cost[i]);
////        sparki.print(' ');
////      }
//      sparki.print(m);
//      sparki.updateLCD();
//    }

    Q_cost[m] = -1;
    //BHEdit
//    for (int neighbor = 0; neighbor < 16; ++neighbor) {
//      int alt = dist[m] + get_travel_cost(m,neighbor);
//      if (get_travel_cost(m,neighbor) != 1) continue;
//      if (alt < dist[neighbor]) {
//        dist[neighbor] = alt;
//        prev[neighbor] = m;
//        Q_cost[neighbor] = alt;        
//      }
//    }
    
    
    
//    iterator++;  // Set an iterator so the function will eventually break, otherwise it runs forever.
//    if (iterator >= 100) break;    
    int i, j;
    if (!vertex_index_to_ij_coordinates(m, &i, &j)) continue;
    if (i + 1 < 4 && world_map[j][i+1] == 1) { // Checks if the adjacent cell is in bounds and unoccupied
      int alt = dist[m] + get_travel_cost(m, m+1); // Sets the distance from the current cell to the adjacent cell
      if (alt < dist[m+1]) { // If the new distance is less than the current distance to that cell
        dist[m+1] = alt; // Sets to the new lesser distance
        prev[m+1] = m; // Sets the previous of the adjacent cell to the m cell
        Q_cost[m+1] = dist[m+1]; // Adds the adjacent cell to the queue so the function can repeat.
      }
    }
    if (i - 1 >= 0 && world_map[j][i-1] == 1) {
      int alt = dist[m] + get_travel_cost(m, m-1);
      if (alt < dist[m-1]) {
        dist[m-1] = alt;
        prev[m-1] = m;
        Q_cost[m-1] = dist[m-1];
      }
    }
    if (j + 1 < 4 && world_map[j+1][i] == 1) {
      int alt = dist[m] + get_travel_cost(m, m+4);
      if (alt < dist[m+4]) {
        dist[m+4] = alt;
        prev[m+4] = m;
        Q_cost[m+4] = dist[m+4];
      }
    }
    if (j - 1 >= 0 && world_map[j-1][i] == 1) {
      int alt = dist[m] + get_travel_cost(m, m-4);
      if (alt < dist[m-4]) {
        dist[m-4] = alt;
        prev[m-4] = m;
        Q_cost[m-4] = dist[m-4];
      }
    }
  }
  return prev;
}

// Given a populated 'prev' array, a source vertex, and destination vertex,
// allocate and return an integer array populated with the path from source to destination.
// The first entry of your path should be source_vertex and the last entry should be "-1" 
// to indicate the end of the array since paths can be variable length.
int *reconstruct_path(int *prev, int source_vertex, int dest_vertex) {
  int *final_path = NULL;

  /**
   * TODO: Insert your code here
   */
   final_path = new int[16];
   int u = dest_vertex;
   int i = 0;
   if (prev[dest_vertex] != -1 || dest_vertex == source_vertex) {
    while (u != -1) {
      final_path[i] = u;
      u = prev[u];
      i++;
    }
   }

  return final_path;
}






void loop () {
  unsigned long begin_time = millis();
  unsigned long end_time = 0;
  unsigned long delay_time = 0;
  int *prev = NULL;
  int *path = NULL;


  /**
   * TODO: Populate prev with dijkstra's algorithm, then populate path with reconstruct_path
   */
//   sparki.clearLCD();
//   sparki.println("Hello");
//   sparki.updateLCD();
   int m = ij_coordinates_to_vertex_index(source_i, source_j);
   int d = ij_coordinates_to_vertex_index(goal_i, goal_j);

   prev = run_dijkstra(m);
//   sparki.clearLCD();
//   for (int i=0;i<NUM_X_CELLS*NUM_Y_CELLS;++i){ // When this prints, every element in the array is -1.
//    sparki.print(prev[i]);
//    sparki.print(" ");
//   }
//   sparki.updateLCD();
   path = reconstruct_path(prev, m, d);


  if (prev != NULL) {
    delete prev; 
    prev = NULL; // Once we have the path, don't need to keep prev around in memory anymore.
  }

  sparki.clearLCD();

  // TODO
  // Display the final path in the following format:
  //
  //  Source: (0,0)
  //  Goal: (3,1)
  //  0 -> 1 -> 2 -> 6 -> 7
  sparki.print("Source: (");sparki.print(source_i);sparki.print(","); sparki.print(source_j); sparki.print(")\n");
  sparki.print("Goal: (");sparki.print(goal_i);sparki.print(","); sparki.print(goal_j); sparki.print(")\n");
  int idx = 0;

  while(path[idx] != -1) {
    sparki.print(path[idx]);
    if(path[idx] == 0) break;
    if(path[idx+1] != -1) sparki.print(" -> ");
    idx++;
  }
  
  sparki.updateLCD();

  if (path != NULL) {
    delete path; 
    path=NULL; // Important! Delete the arrays returned from run_dijkstra and reconstruct_path when you're done with them!
  }
  ///////////////////////////////////////////////////  
 
  end_time = millis();
  delay_time = end_time - begin_time;
  if (delay_time < 1000*CYCLE_TIME)
    delay(1000*CYCLE_TIME - delay_time); // each loop takes CYCLE_TIME ms
  else
    delay(10); // Accept some error
}

